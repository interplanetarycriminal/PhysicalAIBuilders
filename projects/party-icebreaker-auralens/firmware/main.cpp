// AuraLens — Party Icebreaker badge firmware (Level 10)
// Board: Seeed XIAO ESP32-C3
// Libs:  Adafruit_SSD1306, Adafruit_GFX, Adafruit_NeoPixel, NimBLE-Arduino
//
// Advertises a 32-bit interest vector. Scans for peers. If our vector AND
// a peer's vector has ≥1 bit in common, pulse a NeoPixel in a color derived
// from the highest matching bit index.
//
// The interest vector and BLE adv address rotate every 5 min so passive
// MAC tracking can't follow you home.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <NimBLEDevice.h>
#include <Preferences.h>

#define OLED_ADDR   0x3C
#define PIN_NEOPIX  2
#define NEOPIX_COUNT 1
#define MANUF_ID    0xA11A  // "AuraLens" — not a registered vendor ID; lab use only

Adafruit_SSD1306 oled(128, 32, &Wire);
Adafruit_NeoPixel pixel(NEOPIX_COUNT, PIN_NEOPIX, NEO_GRB + NEO_KHZ800);
Preferences prefs;

uint32_t interestVector = 0;        // 32 bits, one per tag you set
String displayName = "friend";

static unsigned long lastRotate = 0;
static const unsigned long ROTATE_MS = 5 * 60 * 1000;

static uint32_t hsvColor(uint8_t hue) {
  // simple HSV → RGB, S=V=1
  uint8_t r, g, b;
  uint8_t region = hue / 43;
  uint8_t remainder = (hue - region * 43) * 6;
  switch (region) {
    case 0: r = 255; g = remainder; b = 0; break;
    case 1: r = 255 - remainder; g = 255; b = 0; break;
    case 2: r = 0; g = 255; b = remainder; break;
    case 3: r = 0; g = 255 - remainder; b = 255; break;
    case 4: r = remainder; g = 0; b = 255; break;
    default: r = 255; g = 0; b = 255 - remainder; break;
  }
  return pixel.Color(r, g, b);
}

class ScanCb : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* dev) override {
    if (!dev->haveManufacturerData()) return;
    std::string md = dev->getManufacturerData();
    if (md.size() < 6) return;
    uint16_t id = (uint8_t)md[0] | ((uint8_t)md[1] << 8);
    if (id != MANUF_ID) return;
    uint32_t peer = (uint8_t)md[2] | ((uint8_t)md[3] << 8)
                  | ((uint8_t)md[4] << 16) | ((uint8_t)md[5] << 24);
    uint32_t match = peer & interestVector;
    if (match == 0) return;
    // pulse color based on highest-matching bit
    int bit = 31 - __builtin_clz(match);
    uint8_t hue = (bit * 8) % 255;
    for (int i = 0; i < 3; ++i) {
      pixel.setPixelColor(0, hsvColor(hue));
      pixel.show();
      delay(140);
      pixel.setPixelColor(0, 0);
      pixel.show();
      delay(100);
    }
  }
};

void startAdvertise() {
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->stop();

  NimBLEAdvertisementData data;
  uint8_t md[6];
  md[0] = MANUF_ID & 0xFF; md[1] = (MANUF_ID >> 8) & 0xFF;
  md[2] = interestVector & 0xFF;
  md[3] = (interestVector >> 8) & 0xFF;
  md[4] = (interestVector >> 16) & 0xFF;
  md[5] = (interestVector >> 24) & 0xFF;
  data.setManufacturerData(std::string((char*)md, 6));
  adv->setAdvertisementData(data);
  adv->start();
}

void rotateIdentity() {
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);
  startAdvertise();
  lastRotate = millis();
}

void setup() {
  Serial.begin(115200);
  pixel.begin(); pixel.setBrightness(40); pixel.show();

  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.clearDisplay();
  oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0); oled.println("AuraLens");
  oled.println("booting...");
  oled.display();

  prefs.begin("aura", false);
  interestVector = prefs.getUInt("vec", 0);
  displayName = prefs.getString("name", "friend");

  NimBLEDevice::init(displayName.c_str());
  NimBLEDevice::setPower(ESP_PWR_LVL_P6);

  NimBLEScan* scan = NimBLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new ScanCb(), false);
  scan->setActiveScan(false);
  scan->setInterval(160);
  scan->setWindow(80);
  scan->start(0, nullptr, false);  // scan forever

  rotateIdentity();

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Hi, "); oled.println(displayName);
  oled.print("tags: 0x"); oled.println(interestVector, HEX);
  oled.display();
}

void loop() {
  if (millis() - lastRotate > ROTATE_MS) {
    rotateIdentity();
  }
  delay(200);
}
