# Hardware encyclopedia

The ESP32 is a general-purpose radio-equipped brain. This directory is the parts bin that teaches it to perceive and act. Every page is cross-referenced with the seven base capabilities in [`../capabilities/`](../capabilities/).

## Reality check: what the May 29 kit actually covers

This encyclopedia is **aspirational** — it maps the whole landscape, far beyond any one kit. The
hardware physically on the bench for the [build night](../build-night/) is a single **Keyestudio
42-in-1 kit** plus classic / S3 / C3 ESP32 boards. Here is how that kit lands on the categories
below; the authoritative part list is [`../build-night/inventory.md`](../build-night/inventory.md).

| Category | In the May 29 kit? |
|---|---|
| [sensors/environmental](sensors/environmental.md) | 🧰 Partial — NTC thermistor, XHT11 (DHT11), 18B20 (temp/humidity only) |
| [sensors/motion](sensors/motion.md) | 🧰 ADXL345 + PIR / tilt / collision / reed / hall |
| [sensors/audio](sensors/audio.md) | 🧰 Sound Sensor (level); buzzer + speaker for output |
| [sensors/chemical](sensors/chemical.md) | 🧰 Gas / alcohol / steam (5V, divide the output) |
| [sensors/radar-lidar](sensors/radar-lidar.md) | 🧰 Ultrasonic only (HC-SR04); no mmWave/lidar |
| [sensors/spectral-light](sensors/spectral-light.md) | 🧰 Photoresistor / flame / IR / laser |
| [sensors/vision](sensors/vision.md) | 🛰️ Aspirational — no camera (ESP32-CAM off-limits) |
| [sensors/bio](sensors/bio.md) | 🛰️ Aspirational — none |
| [sensors/positioning](sensors/positioning.md) | 🛰️ Aspirational — no GPS/UWB/magnetometer |
| [sensors/rf-wireless-csi](sensors/rf-wireless-csi.md) | 🛰️ No LoRa/UWB modules; radio is built in (CSI is a stretch) |
| [addons/displays](addons/displays.md) | 🧰 LCD128x32, HT16K33 8×8, TM1650, 6812 RGB, LEDs |
| [addons/actuators](addons/actuators.md) | 🧰 Servo, motor, fan, relay, laser, buzzer (5V via expansion) |
| [addons/connectivity](addons/connectivity.md) | 🧰 On-board Wi-Fi/BLE (+ BT Classic on classic only) |
| [addons/power](addons/power.md) | 🧰 AA pack + expansion-board rail (no LiPo/solar/PMIC) |
| [addons/compute-accelerators](addons/compute-accelerators.md) | 🛰️ Aspirational — none (the S3 itself is the only "accelerator") |

**Convention:** pages with a kit part carry a 🧰 **In the May 29 kit** note near the top; categories
the kit doesn't reach carry a 🛰️ **Not in the May 29 kit (aspirational)** note. What you can actually
*build* tonight from these parts lives in the [`kit-` projects](../projects/) and the
[templates catalogue](../build-night/templates-catalogue.md).

## Layout

```
hardware/
├── esp32-variants.md                  # which brain to pick
├── sensors/
│   ├── environmental.md               # temp, humid, baro, VOC, PM, CO2, NOx
│   ├── motion.md                      # IMUs, vibration
│   ├── vision.md                      # cameras, ToF, event cameras
│   ├── audio.md                       # MEMS mics, ultrasonic, bone-conduction
│   ├── bio.md                         # HRV, ECG, EMG, GSR, SpO2
│   ├── rf-wireless-csi.md             # RF sensing, CSI, LoRa, UWB
│   ├── chemical.md                    # specific gases, scent, olfaction
│   ├── positioning.md                 # GPS, UWB, magnetometer, dead-reckoning
│   ├── radar-lidar.md                 # mmWave, ToF, lidar
│   └── spectral-light.md              # visible, UV, IR, multispectral, lux
└── addons/
    ├── displays.md                    # OLED, e-paper, TFT, NeoPixel, LED matrix
    ├── actuators.md                   # motors, servos, solenoids, haptic, IR
    ├── connectivity.md                # Wi-Fi / BLE / LoRa / Matter / Thread / ESP-NOW / cellular
    ├── power.md                       # batteries, solar, PMIC, low-power modes
    └── compute-accelerators.md        # TinyML-friendly co-processors, AI cams
```

## How to read the sensor pages

Each sensor page follows the same structure:

1. **What it measures** — the physical quantity.
2. **Part numbers** — the specific ICs / breakouts people actually buy.
3. **Interface** — I²C, SPI, analog, UART, USB.
4. **Capability contribution** — which of the seven verbs this sensor naturally feeds.
5. **Pairs with** — a curated list of combinations that create asymmetry.
6. **Where to buy** — vendor names, not speculative links. Verify current stock before you buy.
7. **Gotchas** — the thing you wish you'd known.

## How to read the add-on pages

Add-on pages share this shape too, plus an **emotional contract** field for displays/haptics (see [`../capabilities/represent.md`](../capabilities/represent.md) — choose your Represent add-on for the feel as much as the function).
