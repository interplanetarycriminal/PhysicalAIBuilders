# Compute accelerators & AI co-processors

When the ESP32's own compute isn't enough: TinyML-friendly accelerators, dedicated AI cameras, and pairable host compute.

## On-device: what the ESP32 itself can do

- **ESP32-S3** has SIMD-ish vector instructions that TFLite Micro and ESP-DL exploit for int8 matmul → ~10× faster than ESP32.
- **ESP-DL** (Espressif's official DL library) — pre-optimized kernels for S3.
- **TFLite Micro** — works on all variants; ~20–100 KB models are viable.
- **Edge Impulse** exports models tuned for ESP32 family.
- **Useful Sensors' models** (Person Sensor, Code Reader) offload from the ESP32.

Model-size rule of thumb: an ESP32-S3 with 8 MB PSRAM can run ~1–3 MB int8 models at 5–15 fps depending on input shape.

## Off-device: co-processors worth knowing

| Module | What it is | Interface | Note |
|---|---|---|---|
| **Grove Vision AI V2 (Himax WiseEye2)** | On-module CNN inference (YOLO-ish) | I²C / UART | Offload vision entirely; ESP32 consumes bounding boxes. |
| **Useful Sensors Person Sensor** | Face detection module | I²C | Privacy-forward: no raw pixels leave the module. |
| **Maxim MAX78000** | Neural-network microcontroller | Pair via SPI | Dedicated CNN accelerator, extreme low power. |
| **Coral Edge TPU (USB)** | Google accelerator | USB | Pair with an ESP32-S3 via USB host, or via a Pi. |
| **Raspberry Pi 5 + AI Kit** | Host companion | Ethernet / USB / Wi-Fi | Where heavier CV / LLMs live in most hybrid setups. |
| **Jetson Orin Nano** | NVIDIA edge GPU | Ethernet | When you want to host a local VLM / LLM. |
| **Ollama / llama.cpp on a home server** | Local LLM host | HTTP | ESP32 → router → local LLM. |

## Capability contribution

- **Transform / Find / Reason** — most computer vision and NLP work sits off-device in a pairing like:
  - ESP32 is the eye/ear and the actuator.
  - Co-processor is the brain.

## Pairs with (asymmetry combos)

- **ESP32-CAM + Grove Vision AI V2** — the ESP32 reads bounding boxes at 15 fps and acts; no pixel ever goes over the network.
- **ESP32-S3 + Coral TPU via USB host** — 100 fps Mobilenet-class vision on a desk.
- **ESP32 node mesh + local Ollama + Home Assistant** — a privacy-preserving "explain my home" agent.
- **XIAO ESP32-S3 Sense + edge-ML model** — wearable vision with everything on-board.

## Where to buy

- **Seeed Studio** for Grove Vision AI V2, XIAO ESP32-S3 Sense.
- **Useful Sensors** for Person Sensor, Tiny Code Reader.
- **DigiKey / Mouser** for Maxim MAX78000.
- **Coral.ai store** for Edge TPU USB.
- **NVIDIA / Seeed / reComputer** for Jetson.

## Gotchas

- **Latency budget**: if you route pixels over USB/Wi-Fi to a co-processor, you've given up the ESP32's latency advantage; pair on-device pre-filter (motion trigger) with off-device heavy model.
- **Thermal**: Jetsons and TPUs throttle; enclose and ventilate.
- **Privacy by design**: prefer modules that output decisions, not pixels, for any camera-fed build.
