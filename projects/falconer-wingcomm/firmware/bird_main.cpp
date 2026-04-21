// Falconer WingComm — bird tag firmware (Level 60)
// Board: LILYGO T-Beam v1.1 (ESP32 + NEO-M8N + SX1276)
// Libs:  RadioLib, TinyGPS++, Adafruit_BMP3XX, SparkFun_ICM20948, LowPower

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>
#include <TinyGPS++.h>
#include <Adafruit_BMP3XX.h>
#include <ICM_20948.h>

// T-Beam pinout
#define LORA_CS   18
#define LORA_RST  23
#define LORA_DIO0 26
#define LORA_DIO1 33
#define GPS_RX    34
#define GPS_TX    12

SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1);
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;
Adafruit_BMP3XX bmp;
ICM_20948_I2C imu;

struct __attribute__((packed)) BirdPacket {
  uint16_t seq;
  float lat, lon;
  int16_t alt_m;
  int8_t vspeed_dmps;   // decimeters/s, fits in int8
  uint16_t heading_cdeg; // centidegrees 0..35999
  uint8_t g_peak;       // latched stoop flag
  uint8_t bat_pct;
};

static uint16_t seq = 0;
static float lastAlt = NAN;
static uint32_t lastAltTs = 0;
static bool stoopLatched = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SPI.begin();

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  if (bmp.begin_I2C(0x77)) {
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_25_HZ);
  }
  imu.begin(Wire);

  int16_t st = radio.begin(868.0, 125.0, 9, 7, 0x12, 17);  // EU 868; US pick 915
  if (st != RADIOLIB_ERR_NONE) Serial.printf("LoRa init err %d\n", st);
}

void loop() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());

  if (bmp.performReading()) {
    float alt = bmp.readAltitude(1013.25);
    uint32_t now = millis();
    if (!isnan(lastAlt) && now > lastAltTs) {
      float dt = (now - lastAltTs) / 1000.0f;
      float vspeed = (alt - lastAlt) / dt;
      if (vspeed < -15.0f) stoopLatched = true;
    }
    lastAlt = alt; lastAltTs = now;
  }

  if (gps.location.isUpdated() && (millis() % 200 < 50)) {
    BirdPacket p;
    p.seq = seq++;
    p.lat = gps.location.lat();
    p.lon = gps.location.lng();
    p.alt_m = isnan(lastAlt) ? 0 : (int16_t)lastAlt;
    p.vspeed_dmps = 0;  // omitted here; see extension
    p.heading_cdeg = (uint16_t)(gps.course.deg() * 100.0);
    p.g_peak = stoopLatched ? 1 : 0;
    p.bat_pct = 80;  // TODO: read from AXP192 on T-Beam

    radio.transmit((uint8_t*)&p, sizeof(p));
    stoopLatched = false;  // clear after transmission
  }
}
