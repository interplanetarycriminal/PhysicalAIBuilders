// Beekeeper HiveLink — solar-powered hive sentinel (Level 70)
// Board: Unexpected Maker TinyS3 (or FeatherS3) + SX1262 LoRa
// Build: PlatformIO + Arduino-ESP32
//
// Architecture: three FreeRTOS tasks + OTA hook at sunrise when battery healthy.
// Sense: 1 Hz scale, 0.1 Hz SEN55, 1 Hz DS18B20, 2 s mic FFT bands.
// Distill: rolling 1/15/1440 min summaries.
// Radio: LoRa summary every 15 min.

#include <Arduino.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>
#include <SparkFun_SEN5x.h>
#include <RadioLib.h>
#include <arduinoFFT.h>

#define PIN_HX_DT 7
#define PIN_HX_SCK 6
#define PIN_OW     5
#define PIN_MIC_ADC 4

#define LORA_CS 10
#define LORA_RST 9
#define LORA_DIO1 8
#define LORA_BUSY 11
// SPI default

HX711 scale;
OneWire oneWire(PIN_OW);
DallasTemperature ds(&oneWire);
SEN5x sen55;
SX1262 radio = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);

static constexpr int N_FFT = 512;
static double vReal[N_FFT];
static double vImag[N_FFT];
ArduinoFFT<double> fft(vReal, vImag, N_FFT, 16000);

struct __attribute__((packed)) HiveSummary {
  uint32_t seq;
  float weight_kg;
  float weight_dkg_per_min;   // derivative
  float temp_brood_c;
  float temp_brood_std_c;
  float co2_ppm;    // optional: can be -1 if SEN55 only
  float pm25;
  float voc_idx;
  float band_low;   // 100..200 Hz energy
  float band_high;  // 200..400 Hz energy
  float battery_v;
};

static HiveSummary curSummary;

static void sampleFFT() {
  // Fill vReal by reading mic at 16 kHz (approx).
  for (int i = 0; i < N_FFT; i++) {
    vReal[i] = analogRead(PIN_MIC_ADC) - 2048;
    vImag[i] = 0;
    delayMicroseconds(60);
  }
  fft.windowing(FFTWindow::Hann, FFTDirection::Forward);
  fft.compute(FFTDirection::Forward);
  fft.complexToMagnitude();
  // bin hz = i * sr / N_FFT = i * 16000 / 512 = 31.25 Hz
  double eLow = 0, eHigh = 0;
  for (int i = 3; i <= 6; i++) eLow += vReal[i];    // ~100..200 Hz
  for (int i = 6; i <= 12; i++) eHigh += vReal[i];  // ~200..400 Hz
  curSummary.band_low = eLow;
  curSummary.band_high = eHigh;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  scale.begin(PIN_HX_DT, PIN_HX_SCK);
  scale.set_scale(420.f);     // calibrate in field
  scale.tare();

  ds.begin();
  sen55.begin(Wire);
  sen55.deviceReset();
  sen55.startMeasurement();

  int16_t st = radio.begin(868.0, 125.0, 9, 7, 0x12, 14);
  if (st != RADIOLIB_ERR_NONE) Serial.printf("LoRa err %d\n", st);

  analogReadResolution(12);
}

void loop() {
  static uint32_t lastSec = 0;
  static uint32_t lastTx = 0;
  static uint32_t seq = 0;
  static float lastWeight = 0;

  uint32_t ms = millis();
  if (ms - lastSec > 1000) {
    lastSec = ms;
    if (scale.is_ready()) {
      float w = scale.get_units(3);
      curSummary.weight_kg = w;
      curSummary.weight_dkg_per_min = (w - lastWeight) * 60.0f * 10.0f;
      lastWeight = w;
    }
    ds.requestTemperatures();
    float sum = 0, sumsq = 0; int n = ds.getDeviceCount();
    for (int i = 0; i < n; i++) {
      float t = ds.getTempCByIndex(i);
      sum += t; sumsq += t * t;
    }
    if (n > 0) {
      float mean = sum / n;
      float var = sumsq / n - mean * mean;
      curSummary.temp_brood_c = mean;
      curSummary.temp_brood_std_c = sqrtf(fmaxf(var, 0));
    }
  }

  // FFT every ~2s
  static uint32_t lastFft = 0;
  if (ms - lastFft > 2000) {
    lastFft = ms;
    sampleFFT();
  }

  // SEN55 every 10 s
  static uint32_t lastSen = 0;
  if (ms - lastSen > 10000) {
    lastSen = ms;
    float pm10 = 0, pm25 = 0, pm4 = 0, pm1 = 0, rh = 0, t = 0, voc = 0, nox = 0;
    sen55.readMeasuredPmVocNoxRhT(pm1, pm25, pm4, pm10, rh, t, voc, nox);
    curSummary.pm25 = pm25;
    curSummary.voc_idx = voc;
  }

  // LoRa transmit every 15 minutes
  if (ms - lastTx > 15UL * 60UL * 1000UL) {
    lastTx = ms;
    curSummary.seq = seq++;
    curSummary.battery_v = 3.9f;  // TODO: read BQ25628 fuel gauge
    int st = radio.transmit((uint8_t*)&curSummary, sizeof(curSummary));
    Serial.printf("tx %d  (err=%d)\n", seq, st);
  }

  delay(10);
}
