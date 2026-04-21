// FermentDoc — brewer's batch logger firmware (Level 20)
// Board: ESP32 DevKitC
// Libs:  OneWire, DallasTemperature, Adafruit_BME280, PubSubClient, Preferences

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_BME280.h>
#include <PubSubClient.h>
#include <Preferences.h>

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;

#define PIN_ONEWIRE 4
#define PIN_BOOT_BTN 0
#define PIN_STATUS_LED 2

OneWire oneWire(PIN_ONEWIRE);
DallasTemperature ds(&oneWire);
Adafruit_BME280 bme;
WiFiClient net;
PubSubClient mqtt(net);
Preferences prefs;

String batchId = "batch-000";
unsigned long lastPublish = 0;
const unsigned long PUBLISH_MS = 30000;

static void ensureWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(200);
  }
}

static void ensureMqtt() {
  if (mqtt.connected()) return;
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  String cid = "fermentdoc-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  mqtt.connect(cid.c_str());
}

static void publishF(const char* metric, float v) {
  char topic[64]; snprintf(topic, sizeof(topic), "fermentdoc/%s/%s", batchId.c_str(), metric);
  char payload[32]; snprintf(payload, sizeof(payload), "%.3f", v);
  mqtt.publish(topic, payload, true);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BOOT_BTN, INPUT_PULLUP);
  pinMode(PIN_STATUS_LED, OUTPUT);

  prefs.begin("ferment", false);
  batchId = prefs.getString("batch", "batch-000");

  Wire.begin();
  ds.begin();
  if (!bme.begin(0x76) && !bme.begin(0x77)) {
    Serial.println("BME280 not found");
  }

  ensureWifi();
  ensureMqtt();
}

void loop() {
  // Long-press BOOT to roll the batch id.
  if (digitalRead(PIN_BOOT_BTN) == LOW) {
    delay(800);
    if (digitalRead(PIN_BOOT_BTN) == LOW) {
      int n = prefs.getInt("n", 0) + 1;
      prefs.putInt("n", n);
      char bid[16]; snprintf(bid, sizeof(bid), "batch-%03d", n);
      batchId = bid;
      prefs.putString("batch", batchId);
      Serial.printf("new batch %s\n", batchId.c_str());
      for (int i = 0; i < 6; ++i) { digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED)); delay(80); }
    }
  }

  if (millis() - lastPublish > PUBLISH_MS) {
    lastPublish = millis();
    ensureWifi();
    ensureMqtt();

    ds.requestTemperatures();
    float t_wort = ds.getTempCByIndex(0);
    float t_amb = bme.readTemperature();
    float rh = bme.readHumidity();
    float p = bme.readPressure() / 100.0f;  // hPa

    publishF("temp_wort", t_wort);
    publishF("temp_amb", t_amb);
    publishF("rh", rh);
    publishF("p", p);

    Serial.printf("[%s] wort=%.2fC amb=%.2fC rh=%.1f p=%.1fhPa\n",
                  batchId.c_str(), t_wort, t_amb, rh, p);
  }

  mqtt.loop();
  delay(100);
}
