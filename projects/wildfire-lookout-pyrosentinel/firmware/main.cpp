// PyroSentinel — ridge-top wildfire sentinel firmware (Level 90)
// Board: ESP32-S3-DevKitC-1 (N16R8)
// Build: PlatformIO (arduino framework on ESP32-S3 OK for this sketch)
//
// Dual-core split:
//   Core 1 (APP_CPU): MLX90640 frame processing + plume-feature extraction
//   Core 0 (PRO_CPU): Sensor polling, LoRa, FOTA, watchdog
//
// LoRa payload packs every 60 s or on confident plume event.

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>
#include <Adafruit_MLX90640.h>
#include <Adafruit_BME280.h>
#include <SparkFun_SEN5x.h>
#include <SparkFun_AS3935.h>
#include <AS5600.h>

#define PIN_ANEMO   5
#define LORA_CS    10
#define LORA_RST    9
#define LORA_BUSY   8
#define LORA_DIO1  11

SX1262 radio = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);
Adafruit_MLX90640 mlx;
Adafruit_BME280 bme;
SEN5x sen55;
SparkFun_AS3935 lightning;
AS5600 vane;

static float mlxFrame[32 * 24];
static volatile float plumeScore = 0;   // 0..1 rough smoke-likelihood (thermal)
static volatile float plumeBearingDeg = -1;

static volatile uint32_t anemoPulses = 0;
void IRAM_ATTR anemoIsr() { anemoPulses++; }

static void processThermal() {
  if (mlx.getFrame(mlxFrame) != 0) return;
  float mean = 0;
  for (int i = 0; i < 32*24; i++) mean += mlxFrame[i];
  mean /= (32*24);

  // Find hotspot cluster — pixels > mean+4 °C, count + centroid.
  int n = 0; float cx = 0, cy = 0, peak = mean;
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 32; x++) {
      float t = mlxFrame[y*32+x];
      if (t > peak) peak = t;
      if (t > mean + 4.0f) {
        cx += x; cy += y; n++;
      }
    }
  }
  if (n == 0) { plumeScore = 0; plumeBearingDeg = -1; return; }
  cx /= n; cy /= n;
  float size = (float)n / (32.0f * 24.0f);
  float excess = peak - mean;
  plumeScore = fminf(1.0f, size * (excess / 10.0f));
  // bearing 0 = centre of frame, negative = left, positive = right; map to degrees
  plumeBearingDeg = (cx - 16.0f) / 16.0f * 40.0f;   // ±40° FOV mapping
}

static void thermalTask(void* arg) {
  while (true) {
    processThermal();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

struct __attribute__((packed)) SentinelPacket {
  uint32_t node_id;
  uint32_t seq;
  float t_c, rh, p_hpa;
  float pm25, voc, nox;
  float wind_mps, wind_dir_deg;
  float plume_score, plume_bearing_deg;
  uint8_t lightning_10km_60min;
  float battery_v;
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SPI.begin();

  mlx.begin(0x33, &Wire);
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_2_HZ);

  bme.begin(0x76);
  sen55.begin(Wire); sen55.startMeasurement();
  lightning.begin(); lightning.setIndoorOutdoor(OUTDOOR); lightning.setNoiseLevel(3);
  vane.begin();

  pinMode(PIN_ANEMO, INPUT_PULLUP);
  attachInterrupt(PIN_ANEMO, anemoIsr, FALLING);

  if (radio.begin(868.0, 125.0, 9, 7, 0x12, 22) != RADIOLIB_ERR_NONE) {
    Serial.println("LoRa init err");
  }

  xTaskCreatePinnedToCore(thermalTask, "therm", 8192, nullptr, 1, nullptr, APP_CPU_NUM);
}

void loop() {
  static uint32_t lastMin = 0;
  static uint32_t lastPulse = 0;
  static uint32_t seq = 0;
  uint32_t ms = millis();

  if (ms - lastMin > 60000) {
    uint32_t delta = anemoPulses - lastPulse; lastPulse = anemoPulses;
    float wind_mps = delta * 0.666f / 60.0f;  // 1 pulse/s ≈ 0.666 m/s (anemometer constant)

    float pm1, pm25, pm4, pm10, rh, t, voc, nox;
    sen55.readMeasuredPmVocNoxRhT(pm1, pm25, pm4, pm10, rh, t, voc, nox);

    SentinelPacket p = {};
    p.node_id = ESP.getEfuseMac() & 0xFFFFFFFF;
    p.seq = seq++;
    p.t_c = bme.readTemperature();
    p.rh = bme.readHumidity();
    p.p_hpa = bme.readPressure() / 100.0;
    p.pm25 = pm25;
    p.voc = voc;
    p.nox = nox;
    p.wind_mps = wind_mps;
    p.wind_dir_deg = (vane.readAngle() / 4096.0f) * 360.0f;
    p.plume_score = plumeScore;
    p.plume_bearing_deg = plumeBearingDeg;
    p.lightning_10km_60min = 0; // integrate lightning counts in a real build
    p.battery_v = 13.2f;        // TODO: read BMS

    radio.transmit((uint8_t*)&p, sizeof(p));
    lastMin = ms;
  }

  delay(100);
}
