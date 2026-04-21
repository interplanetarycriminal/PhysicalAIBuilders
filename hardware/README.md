# Hardware encyclopedia

The ESP32 is a general-purpose radio-equipped brain. This directory is the parts bin that teaches it to perceive and act. Every page is cross-referenced with the seven base capabilities in [`../capabilities/`](../capabilities/).

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
