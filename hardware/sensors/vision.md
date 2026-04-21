# Vision sensors

Cameras (visible and thermal), ToF depth, event cameras. The "what can it see?" stack.

## Part catalog

| Part | Kind | Interface | Note |
|---|---|---|---|
| **OV2640** | 2 MP RGB camera | Parallel DVP | Classic ESP32-CAM sensor; fine for 640×480 MJPEG streaming. |
| **OV3660 / OV5640** | 3–5 MP RGB | Parallel DVP / MIPI | Better optics, more PSRAM needed. Works with ESP32-S3 or ESP32-P4. |
| **Himax HM0360** | Low-power monochrome | Parallel | Always-on "person waiting?" vision; pairs with S3 + TinyML. |
| **Useful Sensors Person Sensor** | Face detection module | I²C | All-in-one; returns bounding boxes + identity slots. |
| **Useful Sensors Tiny Code Reader** | QR decoder module | I²C | Trade a DIY camera pipeline for a dedicated module. |
| **Grove Vision AI V2** | Himax WiseEye2 + YOLO | I²C / UART | On-module CNN; offloads vision from the ESP32. |
| **MLX90640 / MLX90641** | Thermal 24×32 / 16×12 | I²C | Affordable thermal imaging, 1–8 fps. |
| **AMG8833** | Thermal 8×8 | I²C | Ultra-cheap IR grid; presence + gross heat scanning. |
| **Lepton 3.5 / FLIR Lepton** | 160×120 thermal | SPI | Pro thermal; more expensive but beautiful. |
| **VL53L5CX** | 8×8 multi-zone ToF | I²C | Eight-by-eight tiny depth "camera"; amazing for gesture / presence zones. |
| **VL53L1X / VL53L0X** | Single-point ToF | I²C | Long- and short-range distance beams. |
| **Prophesee EVK / event camera** | Event-based vision | MIPI | Microsecond latency, asynchronous pixels; requires heavier compute (P4 or host). |

## Interface notes

- Parallel DVP cameras eat GPIO — the ESP32-CAM wires most of them; don't expect many free pins.
- ESP32-S3 has **LCD_CAM** peripheral that makes OV2640/OV5640 much cleaner.
- ESP32-P4 supports **MIPI-CSI** directly — think about this when you need bigger sensors.
- PSRAM is your friend: 8 MB for smooth MJPEG streaming.

## Capability contribution

- **Find**: person, face, QR, motion, object class.
- **Transform**: image → embedding, image → depth (MiDaS host-side), image → thermal-labeled scene.
- **Distill**: "how many people walked through the garage today".
- **Reason**: multi-modal (image + audio + IMU) via a VLM off-device.
- **Represent**: annotated stream on TFT / web UI.

## Pairs with (asymmetry combos)

- **OV2640 + MLX90640 + mmWave** — RGB + thermal + radar presence. You see people, their heat signature, and whether they're breathing. (Baby monitor done right.)
- **Himax HM0360 + BLE scanner** — wake the S3 only when a human is visually present *and* there's a known phone nearby.
- **VL53L5CX array mounted on a door** — who went in, which direction, count.
- **Event camera + IMU** — balance / fall detection that doesn't blur in low light.

## Where to buy

- **AI Thinker / generic ESP32-CAM** — cheap, everywhere; verify with an Espressif-ish vendor.
- **Seeed Studio** for XIAO ESP32-S3 Sense (camera + mic combo board).
- **Adafruit** for MLX90640 breakouts.
- **DigiKey / Mouser** for MLX / VL53Lxx / FLIR Lepton.
- **Useful Sensors** for Person Sensor & Tiny Code Reader.
- **Seeed** for Grove Vision AI V2.
- **Prophesee** for event-camera EVKs.

## Gotchas

- **Thermal sensors want line-of-sight** — glass blocks mid-wave IR.
- **MJPEG streaming** over Wi-Fi from ESP32-CAM is reliable up to ~15 fps at 640×480; beyond that, PSRAM runs out.
- **FLIR Lepton calibration shutter** clacks audibly once a minute — budget that noise into the UX.
- **Privacy**: any camera on any build needs a hardware LED and a physical shutter. No exceptions.
