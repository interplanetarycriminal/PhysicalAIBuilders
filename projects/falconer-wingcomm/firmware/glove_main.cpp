// Falconer WingComm — glove console firmware (Level 60)
// Board: LILYGO T-Display-S3 with LoRa shield
// Libs:  RadioLib, TFT_eSPI, Adafruit_DRV2605, TinyGPS++ (for glove GPS if present)

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>
#include <TFT_eSPI.h>
#include <Adafruit_DRV2605.h>

#define LORA_CS   10
#define LORA_RST  15
#define LORA_DIO0 16
#define LORA_DIO1 17

SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1);
TFT_eSPI tft;
Adafruit_DRV2605 haptic;

struct __attribute__((packed)) BirdPacket {
  uint16_t seq;
  float lat, lon;
  int16_t alt_m;
  int8_t vspeed_dmps;
  uint16_t heading_cdeg;
  uint8_t g_peak;
  uint8_t bat_pct;
};

static uint32_t lastRx = 0;
static float gloveLat = 0, gloveLon = 0;  // TODO: inject from phone or onboard GPS

static void drawMap(const BirdPacket& p) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(4, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.printf("alt %d m", p.alt_m);
  tft.setCursor(4, 24);
  tft.printf("hdg %d", p.heading_cdeg / 100);
  tft.setCursor(4, 44);
  tft.printf("bat %d%%", p.bat_pct);
  // crude glove-centric dot
  int cx = tft.width()/2, cy = tft.height()/2;
  tft.drawCircle(cx, cy, 4, TFT_GREEN);
  float dx = (p.lon - gloveLon) * 1000.0f;
  float dy = (p.lat - gloveLat) * 1000.0f;
  int px = cx + (int)(dx);
  int py = cy - (int)(dy);
  tft.fillCircle(px, py, 3, p.g_peak ? TFT_RED : TFT_ORANGE);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SPI.begin();

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  haptic.begin();
  haptic.selectLibrary(1);
  haptic.setMode(DRV2605_MODE_INTTRIG);

  int16_t st = radio.begin(868.0, 125.0, 9, 7, 0x12, 17);
  if (st != RADIOLIB_ERR_NONE) {
    tft.setCursor(4, 80); tft.printf("LoRa err %d", st);
  }
  radio.startReceive();
}

void loop() {
  if (radio.readData((uint8_t*)nullptr, 0) == RADIOLIB_ERR_NONE) {
    BirdPacket p;
    int st = radio.readData((uint8_t*)&p, sizeof(p));
    if (st == RADIOLIB_ERR_NONE) {
      lastRx = millis();
      drawMap(p);
      if (p.g_peak) { haptic.setWaveform(0, 47); haptic.setWaveform(1, 0); haptic.go(); }
    }
    radio.startReceive();
  }

  if (millis() - lastRx > 2000) {
    haptic.setWaveform(0, 1); haptic.setWaveform(1, 0); haptic.go();
    lastRx = millis() + 500;   // rate-limit the "lost-the-bird" haptic
  }
}
