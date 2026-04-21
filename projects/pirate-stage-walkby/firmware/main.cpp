// Pirate Stage Walk-By — ESP32-CAM firmware (Level 30, creative track)
// Board: AI-Thinker ESP32-CAM
// Libs:  esp32-camera (bundled), WebSocketsServer (Links2004), WiFi
//
// This firmware is deliberately small. It publishes minimal silhouette
// telemetry over a WebSocket:
//     { "seen": true, "x_norm": 0.42, "v_norm": -0.67 }
// A browser scene does all the imagining. See ../companion/README.md.

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "esp_camera.h"

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";

// AI-Thinker pin map
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

WebSocketsServer ws(81);

// Background model (running mean, per-pixel) at QVGA grayscale.
// Subtract current frame; threshold; compute silhouette centroid + motion.
static const int W = 320;
static const int H = 240;
static float bg[W * H];
static bool bgInit = false;

static float lastCx = 0.5f;
static uint32_t lastTs = 0;

static void initCamera() {
  camera_config_t c = {};
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer   = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM;  c.pin_d1 = Y3_GPIO_NUM;
  c.pin_d2 = Y4_GPIO_NUM;  c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM;  c.pin_d5 = Y7_GPIO_NUM;
  c.pin_d6 = Y8_GPIO_NUM;  c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM; c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM; c.pin_href = HREF_GPIO_NUM;
  c.pin_sccb_sda = SIOD_GPIO_NUM; c.pin_sccb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM; c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.frame_size = FRAMESIZE_QVGA;
  c.pixel_format = PIXFORMAT_GRAYSCALE;
  c.grab_mode = CAMERA_GRAB_LATEST;
  c.fb_location = CAMERA_FB_IN_PSRAM;
  c.jpeg_quality = 12;
  c.fb_count = 2;
  if (esp_camera_init(&c) != ESP_OK) {
    Serial.println("camera init failed");
    delay(2000);
    ESP.restart();
  }
}

static void processFrame(camera_fb_t* fb) {
  if (!fb || fb->len < (size_t)(W * H)) return;
  const uint8_t* px = fb->buf;

  // Update running background + compute silhouette stats
  float sumX = 0;
  int count = 0;
  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      int i = y * W + x;
      float v = px[i];
      if (!bgInit) { bg[i] = v; }
      float diff = fabsf(v - bg[i]);
      bool fg = diff > 30.0f;
      if (fg) { sumX += x; count++; }
      bg[i] = bg[i] * 0.98f + v * 0.02f;  // slow background adaptation
    }
  }
  bgInit = true;

  bool seen = count > (W * H) / 80;  // >= ~1.25% of pixels moved
  float cx = seen ? (sumX / count) / (float)W : 0.5f;
  uint32_t now = millis();
  float dt_s = lastTs ? (now - lastTs) / 1000.0f : 0.2f;
  float v_norm = seen ? (cx - lastCx) / dt_s : 0.0f;
  // Clamp v_norm to [-1, 1] for the browser
  if (v_norm > 1) v_norm = 1; if (v_norm < -1) v_norm = -1;
  lastCx = cx;
  lastTs = now;

  char body[128];
  snprintf(body, sizeof(body),
    "{\"seen\":%s,\"x_norm\":%.3f,\"v_norm\":%.3f,\"t\":%u}",
    seen ? "true" : "false", cx, v_norm, now);
  ws.broadcastTXT(body);
}

void setup() {
  Serial.begin(115200);
  initCamera();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }
  Serial.print("IP "); Serial.println(WiFi.localIP());
  ws.begin();
}

void loop() {
  ws.loop();
  camera_fb_t* fb = esp_camera_fb_get();
  if (fb) {
    processFrame(fb);
    esp_camera_fb_return(fb);
  }
  delay(150);  // ~6 fps
}
