// AR Garden Oracle — wand firmware (Level 30)
// Board: Unexpected Maker FeatherS3
// Libs:  Adafruit_AS7341, Adafruit_TSL2591, NimBLE-Arduino
//
// A hand-held wand with AS7341 (11-ch spectral), TSL2591 (lux), and an ADC
// soil-moisture probe. Exposes a BLE GATT service; phone reads the snapshot.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>
#include <Adafruit_TSL2591.h>
#include <NimBLEDevice.h>

#define PIN_BUTTON 0
#define PIN_SOIL   14  // ADC-capable pin on FeatherS3

#define SVC_UUID   "6b5f0001-1234-4321-9abc-aura0000garden"
#define CHR_UUID   "6b5f0002-1234-4321-9abc-aura0000garden"

Adafruit_AS7341 spec;
Adafruit_TSL2591 lux = Adafruit_TSL2591(2591);
NimBLECharacteristic* chr = nullptr;

static String capture() {
  uint16_t s[12];
  if (!spec.readAllChannels(s)) return "{\"err\":\"as7341\"}";
  uint32_t raw = lux.getFullLuminosity();
  uint16_t full = raw & 0xFFFF;
  uint16_t ir = raw >> 16;
  float lx = lux.calculateLux(full, ir);
  int soil = analogRead(PIN_SOIL);

  String j = "{\"ts\":" + String(millis())
           + ",\"lux\":" + String(lx, 1)
           + ",\"soil\":" + String(soil)
           + ",\"spectrum\":[";
  for (int i = 0; i < 12; i++) {
    j += String(s[i]);
    if (i < 11) j += ",";
  }
  j += "]}";
  return j;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_SOIL, INPUT);

  Wire.begin();
  if (!spec.begin()) Serial.println("AS7341 not found");
  spec.setATIME(100);
  spec.setASTEP(999);
  spec.setGain(AS7341_GAIN_256X);

  if (!lux.begin()) Serial.println("TSL2591 not found");
  lux.setGain(TSL2591_GAIN_MED);
  lux.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  NimBLEDevice::init("GardenOracle");
  NimBLEServer* srv = NimBLEDevice::createServer();
  NimBLEService* svc = srv->createService(SVC_UUID);
  chr = svc->createCharacteristic(CHR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  chr->setValue("ready");
  svc->start();

  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID(SVC_UUID);
  adv->start();
}

void loop() {
  if (digitalRead(PIN_BUTTON) == LOW) {
    delay(30);
    if (digitalRead(PIN_BUTTON) == LOW) {
      String snap = capture();
      chr->setValue((uint8_t*)snap.c_str(), snap.length());
      chr->notify();
      Serial.println(snap);
      // debounce
      while (digitalRead(PIN_BUTTON) == LOW) delay(10);
    }
  }
  delay(20);
}
