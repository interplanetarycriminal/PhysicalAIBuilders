# ESP32 variants — which brain to pick

> The ESP32 family has branched a lot since 2016. Picking the wrong variant at the start of a build is a common and expensive mistake. This page is the decision tree.

## Quick picker

| Your build needs… | Pick |
|---|---|
| Cheap, Wi-Fi + BLE, tons of examples, you just want it to work | **ESP32** (the original; "WROOM-32") |
| USB-native, more RAM/flash, vector acceleration for audio/ML | **ESP32-S3** |
| Small, low power, single-core RISC-V, BLE 5 | **ESP32-C3** |
| Wi-Fi 6 + Thread + Zigbee in one (Matter-ready) | **ESP32-C6** |
| Thread / Zigbee only (no Wi-Fi), very low power | **ESP32-H2** |
| Heavy compute, camera pipelines, MIPI-CSI, multiple USB2 — no native radio | **ESP32-P4** (pair with an ESP32-C6 for radio) |
| USB + dual-core but not the latest — cheap bulk projects | **ESP32-S2** (single core, USB) |
| Camera built in, cheap | **ESP32-CAM** (original ESP32 + OV2640) |
| Integrated camera + mic + LCD, for ML-on-camera prototypes | **ESP-EYE** / **ESP32-S3-EYE** / **XIAO ESP32S3 Sense** |

## Why it matters

- **Radio stack is NOT the same across variants.** Wi-Fi CSI works well on the original ESP32; on newer variants, library support is still catching up. If your build depends on CSI, **start with ESP32**.
- **USB device/host** on S2/S3/C3/C6/H2/P4 enables real USB HID, MSC, CDC; great for keyboard-gag builds, MIDI devices, drag-and-drop firmware updates.
- **PSRAM** on S3 (up to 8–16 MB) is essential for camera framebuffers and LVGL-heavy UIs. The C3/C6 typically don't have PSRAM — budget your RAM.
- **Vector extensions (S3)** accelerate int8 matrix multiplies — big deal for TinyML.
- **ESP32-P4** has NO radio. Always pair it with an S3/C6 for wireless. Use P4 when you want a compute-heavy sensor hub.

## Reference boards worth knowing

- **ESP32 DevKitC** — vanilla original ESP32; reference.
- **ESP32-S3-DevKitC-1** — vanilla S3.
- **XIAO ESP32-S3** / **XIAO ESP32-C3** — Seeed's ultra-tiny boards; great for wearables.
- **Unexpected Maker FeatherS3 / TinyS3 / ProS3** — opinionated S3 boards with careful power design.
- **LILYGO T-Display / T-Watch / T-Beam** — S3 with screen / watch form factor / GPS+LoRa.
- **Adafruit Feather ESP32-S3** — the "works first try" pick; LiPo charging built in.
- **M5Stack / M5StickC Plus2** — enclosed ESP32 with display + buttons + IMU in a candy-bar.
- **Heltec / RAK LoRa boards** — ESP32 + LoRa SX126x, ready for mesh / Meshtastic.
- **ESP32-S3-EYE** / **ESP32-S3 CAM (XIAO Sense)** — camera+mic+PSRAM, good for ML demos.

## Power profile cheat sheet

| Mode | Original ESP32 | S3 | C3/C6/H2 |
|---|---|---|---|
| Active (Wi-Fi TX) | 150–240 mA | 150–260 mA | 60–180 mA |
| Modem-sleep | 15–20 mA | 15–25 mA | 10–20 mA |
| Light-sleep | 0.8–1.0 mA | 0.8 mA | 0.2–0.6 mA |
| Deep-sleep (RTC only) | ~10 µA | ~8 µA | ~5 µA |

Pair with a good PMIC (see [`addons/power.md`](addons/power.md)) to make deep-sleep duty cycles buy you months on a LiPo.

## Toolchains

- **Arduino-ESP32** — easiest on-ramp; most of the community code works here.
- **ESP-IDF** — first-class Espressif SDK; required for advanced features (CSI, ESP-NOW low-level, deep RTOS work).
- **PlatformIO** — cross-IDE project management; wraps both.
- **MicroPython / CircuitPython** — fast prototyping, great for kids and art projects.
- **esp-rs (Rust)** — memory-safe firmware; improving rapidly.

## Where to buy (vendors, not links)

- **Espressif reference:** DigiKey, Mouser, Arrow.
- **Maker variants:** Adafruit, SparkFun, Seeed Studio, Unexpected Maker, LILYGO, M5Stack.
- **Bulk / deep options:** AliExpress (verify seller ratings), Taobao.

Tip: buy two of every board. You'll fry one.
