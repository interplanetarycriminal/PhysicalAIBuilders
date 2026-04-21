// Ghost Hunter — ESP-IDF firmware (Level 80)
// Board: ESP32 DevKitC (original)
// Build: esp-idf v5.x + esp-idf-arduino if needed; CSI callback uses native IDF.
//
// This file is an orientation sketch. It wires Wi-Fi CSI collection in
// parallel with LD2410B parsing and exposes a tiny JSON tool-surface over
// HTTP that the companion agent talks to.

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "nvs_flash.h"
#include <string.h>
#include <math.h>

static const char* TAG = "ghost";

static float csi_variance_ring[32];
static size_t csi_idx = 0;
static float last_presence_frac = 0;
static float last_motion_frac = 0;

static void csi_cb(void* ctx, wifi_csi_info_t* info) {
  if (!info || !info->buf || info->len < 128) return;
  float sum = 0, sumsq = 0;
  int8_t* buf = info->buf;
  for (int i = 0; i < info->len; i += 2) {
    float amp = sqrtf((float)buf[i]*buf[i] + (float)buf[i+1]*buf[i+1]);
    sum += amp; sumsq += amp*amp;
  }
  int n = info->len / 2;
  float mean = sum / n;
  float var = sumsq / n - mean*mean;
  csi_variance_ring[csi_idx++ % 32] = var;
}

static void init_wifi_csi() {
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  wifi_csi_config_t csi_cfg = {
    .lltf_en = true, .htltf_en = true, .stbc_htltf2_en = true,
    .ltf_merge_en = true, .channel_filter_en = true, .manu_scale = false,
    .shift = 0,
  };
  esp_wifi_set_csi_config(&csi_cfg);
  esp_wifi_set_csi_rx_cb(csi_cb, NULL);
  esp_wifi_set_csi(true);
}

static esp_err_t tool_scan(httpd_req_t* req) {
  float vmean = 0;
  for (int i = 0; i < 32; i++) vmean += csi_variance_ring[i];
  vmean /= 32.0f;

  char body[256];
  snprintf(body, sizeof(body),
    "{"
      "\"csi_variance\":%.3f,"
      "\"presence_frac\":%.2f,"
      "\"motion_frac\":%.2f,"
      "\"note\":\"non-zero csi variance with mmWave presence agreement \"\n"
              "\"implies a moving body; breath estimation requires a longer window.\""
    "}",
    vmean, last_presence_frac, last_motion_frac);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_sendstr(req, body);
  return ESP_OK;
}

extern "C" void app_main() {
  nvs_flash_init();
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();

  init_wifi_csi();

  // TODO: init Serial2 for LD2410B; update last_presence_frac / last_motion_frac
  // from a FreeRTOS task polling the radar.

  httpd_handle_t server = NULL;
  httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
  if (httpd_start(&server, &cfg) == ESP_OK) {
    httpd_uri_t scan = { .uri="/tools/scan", .method=HTTP_GET, .handler=tool_scan };
    httpd_register_uri_handler(server, &scan);
  }

  while (true) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
