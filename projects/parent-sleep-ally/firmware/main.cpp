// Parent Sleep Ally — firmware (Level 40)
// Board: ESP32 DevKitC
// Build: PlatformIO + Arduino-ESP32
//
// Sensing task: LD2410 (5 Hz), SCD41 (0.1 Hz), mic RMS (2 s windows, no recording).
// Representation task: e-paper redraw at ~06:30 or on parental tap.
// MQTT: publishes per-minute rollups + a night-summary at 06:00.

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <driver/i2s.h>
#include <ld2410.h>
#include <SparkFun_SCD4x_Arduino_Library.h>
#include <time.h>

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;
const char* ROOM = "nursery";

#define I2S_MIC I2S_NUM_0
#define PIN_I2S_WS 25
#define PIN_I2S_SCK 26
#define PIN_I2S_SD 33

ld2410 radar;
SCD4x scd;
WiFiClient net;
PubSubClient mqtt(net);

struct Rollup {
  uint16_t minute;
  float co2_ppm;
  float rh;
  float t_c;
  uint8_t presence_frac;    // 0..255 fraction of second ticks with presence
  uint16_t mic_rms_p95;     // 95th percentile of RMS buckets
  uint16_t loud_events;     // RMS > threshold events
};

static Rollup cur = {0};

static uint16_t mic_rms_window_ms(uint16_t window_ms = 2000) {
  const int samples_per_call = 512;
  int32_t buf[samples_per_call];
  size_t read_bytes = 0;
  uint64_t sq_sum = 0;
  uint32_t total = 0;
  uint32_t start = millis();
  while (millis() - start < window_ms) {
    i2s_read(I2S_MIC, (void*)buf, sizeof(buf), &read_bytes, 20 / portTICK_PERIOD_MS);
    int n = read_bytes / sizeof(int32_t);
    for (int i = 0; i < n; i++) {
      int32_t s = buf[i] >> 14;  // 18-bit effective → scale down
      sq_sum += (int64_t)s * s;
      total++;
    }
  }
  if (!total) return 0;
  return (uint16_t)sqrt((double)sq_sum / total);
}

static void init_i2s_mic() {
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
  cfg.sample_rate = 16000;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.dma_buf_count = 6;
  cfg.dma_buf_len = 256;
  cfg.use_apll = false;
  i2s_pin_config_t pins = { .bck_io_num = PIN_I2S_SCK, .ws_io_num = PIN_I2S_WS,
                            .data_out_num = I2S_PIN_NO_CHANGE, .data_in_num = PIN_I2S_SD };
  i2s_driver_install(I2S_MIC, &cfg, 0, NULL);
  i2s_set_pin(I2S_MIC, &pins);
  i2s_zero_dma_buffer(I2S_MIC);
}

static void ensureWifiMqtt() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) delay(200);
  }
  if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) {
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    char cid[32]; snprintf(cid, 32, "sleep-ally-%06x", (uint32_t)ESP.getEfuseMac() & 0xFFFFFF);
    mqtt.connect(cid);
  }
}

static void publishMinute(const Rollup& r) {
  char topic[64]; snprintf(topic, 64, "sleep-ally/%s/minute", ROOM);
  char body[160];
  snprintf(body, sizeof(body),
    "{\"m\":%u,\"co2\":%.0f,\"rh\":%.1f,\"t\":%.1f,\"pres\":%u,\"mic_p95\":%u,\"loud\":%u}",
    r.minute, r.co2_ppm, r.rh, r.t_c, r.presence_frac, r.mic_rms_p95, r.loud_events);
  mqtt.publish(topic, body, false);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial2.begin(256000, SERIAL_8N1, 16, 17);  // LD2410 default

  if (!radar.begin(Serial2)) Serial.println("LD2410 not found");
  scd.begin(false);
  scd.startPeriodicMeasurement();

  init_i2s_mic();
  ensureWifiMqtt();

  configTime(0, 0, "pool.ntp.org");
}

void loop() {
  uint16_t ticks_with_presence = 0;
  uint32_t mic_p95_sketch[20] = {0};
  uint16_t loud_events = 0;

  for (int s = 0; s < 60; s++) {
    radar.read();
    if (radar.presenceDetected()) ticks_with_presence++;
    uint16_t rms = mic_rms_window_ms(1000);
    uint8_t bucket = min(19, (int)(rms / 50));
    mic_p95_sketch[bucket]++;
    if (rms > 600) loud_events++;
  }

  uint16_t total = 0;
  for (int i = 0; i < 20; ++i) total += mic_p95_sketch[i];
  uint16_t rank = (uint16_t)(total * 0.95f);
  uint16_t seen = 0; uint16_t p95 = 0;
  for (int i = 0; i < 20; ++i) { seen += mic_p95_sketch[i]; if (seen >= rank) { p95 = i * 50; break; } }

  if (scd.readMeasurement()) {
    cur.co2_ppm = scd.getCO2();
    cur.rh = scd.getHumidity();
    cur.t_c = scd.getTemperature();
  }
  cur.presence_frac = ticks_with_presence * 4;  // scale 0..60 → 0..240
  cur.mic_rms_p95 = p95;
  cur.loud_events = loud_events;
  time_t now = time(nullptr);
  struct tm tmnow; localtime_r(&now, &tmnow);
  cur.minute = tmnow.tm_hour * 60 + tmnow.tm_min;

  ensureWifiMqtt();
  publishMinute(cur);
  mqtt.loop();
}
