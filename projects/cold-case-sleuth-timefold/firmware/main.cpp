// TimeFold — per-node firmware (Level 100)
// Board: Seeed XIAO ESP32-S3 Sense
// Build: PlatformIO + Arduino-ESP32
//
// Each node:
//   1) Polls LD2410B presence + BME280 ambient + mic RMS (no recording).
//   2) Every 60 s, publishes MQTT rollup topic: timefold/<node>/minute
//   3) Computes an on-device 16-dim "seed" feature vector used by the
//      gateway to build 128-dim hourly/daily embeddings.
//   4) Exposes HTTP tool surface: /tools/playback, /tools/state

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <ld2410.h>
#include <driver/i2s.h>
#include <ESPAsyncWebServer.h>

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "192.168.1.10";

Adafruit_BME280 bme;
ld2410 radar;
WiFiClient net;
PubSubClient mqtt(net);
AsyncWebServer http(80);

#define PIN_I2S_WS 42
#define PIN_I2S_SCK 41
#define PIN_I2S_SD 2

static float seed[16];   // on-device seed of the per-minute state

static void init_i2s() {
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
  cfg.sample_rate = 16000;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.dma_buf_count = 4;
  cfg.dma_buf_len = 256;
  i2s_pin_config_t pins = { .bck_io_num = PIN_I2S_SCK, .ws_io_num = PIN_I2S_WS,
                            .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = PIN_I2S_SD };
  i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
}

static float mic_rms_1s() {
  const int N = 2048;
  int32_t buf[N];
  size_t got = 0;
  double ss = 0; int total = 0;
  uint32_t t0 = millis();
  while (millis() - t0 < 1000) {
    i2s_read(I2S_NUM_0, buf, sizeof(buf), &got, 20/portTICK_PERIOD_MS);
    int n = got / 4;
    for (int i = 0; i < n; i++) { int32_t s = buf[i] >> 14; ss += (double)s*s; total++; }
  }
  return total ? sqrtf(ss / total) : 0.0f;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial2.begin(256000, SERIAL_8N1, 43, 44);
  radar.begin(Serial2);
  bme.begin(0x76);
  init_i2s();

  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);
  mqtt.setServer(MQTT_HOST, 1883);

  http.on("/tools/state", HTTP_GET, [](AsyncWebServerRequest* req){
    String body = "{\"seed\":["; for (int i=0;i<16;i++){body+=String(seed[i],3);if(i<15)body+=",";}
    body += "]}";
    req->send(200, "application/json", body);
  });
  http.begin();
}

void loop() {
  static uint32_t lastMin = 0;
  uint32_t ms = millis();
  if (ms - lastMin > 60000) {
    lastMin = ms;
    if (!mqtt.connected()) {
      char cid[32]; snprintf(cid,32,"tf-%06x",(uint32_t)ESP.getEfuseMac() & 0xFFFFFF);
      mqtt.connect(cid);
    }
    radar.read();
    float rms = mic_rms_1s();
    float t = bme.readTemperature();
    float rh = bme.readHumidity();
    float p = bme.readPressure() / 100.0f;
    // 16-dim seed (stubs): presence, motion, rms, t, rh, p, and derivatives.
    seed[0] = radar.presenceDetected();
    seed[1] = radar.motionDetected();
    seed[2] = rms;
    seed[3] = t; seed[4] = rh; seed[5] = p;
    // Derivatives left as zero here; gateway computes them from rollups.
    for (int i = 6; i < 16; i++) seed[i] = 0;

    char body[256];
    snprintf(body,sizeof(body),
      "{\"pres\":%d,\"mot\":%d,\"rms\":%.1f,\"t\":%.2f,\"rh\":%.1f,\"p\":%.1f}",
      (int)seed[0],(int)seed[1],seed[2],seed[3],seed[4],seed[5]);
    String topic = "timefold/" + WiFi.macAddress() + "/minute";
    mqtt.publish(topic.c_str(), body, false);
  }
  mqtt.loop();
  delay(50);
}
