# Inventory — what we physically have on the night

> The source of truth for *"is this part actually in the kit?"*. Distilled from the
> [facilitator pack](../MAY29th_README_PhysicalAIBuilders.md) §1. Voltage column: **3V3** = runs and
> signals safely at 3.3V (GPIO-safe). **5V** = needs 5V power *and* you must protect its signal line
> (see [`safety-and-gpio.md`](safety-and-gpio.md)). "Easy" flags the fast, beginner-friendly first wins.

## Boards (two pools + optionals)

| Board | Qty | USB | Role tonight |
|---|---|---|---|
| **ESP32-WROOM-32E** (classic dual-core) | 5 | CP2102/CH340 UART | **The default.** All Keyestudio kit code targets this. Easiest path. |
| **Keyestudio ESP32 Plus** | 5 | CP2102 | Same family as the WROOM-32E — treat identically. The other half of the **classic pool**. |
| **ESP32-S3-WROOM-1** | 5 | Native USB | The **S3 pool**. Use for BLE, native-USB, and "AI agent reacts physically" demos. Different pinout — **no GPIO22** (see safety). |
| ESP32-C3 mini (2-pack) | 2 | varies | Optional only. Fewer GPIOs, different pinout. Not the focus. |
| Freenove ESP32-CAM kit | 1 | CH340 | **Off-limits unless pre-assembled** (may need header soldering). Set aside. |

> **Two pools.** A large **classic** pool (WROOM-32E + Keyestudio Plus) and a **5-strong S3** pool.
> Default everyone to **classic** for the first win.

## Sensors & modules — the Keyestudio 42-in-1 kit (KS5003, ~2 of each)

| Module (kit name) | Volt | Easy | What it is |
|---|---|---|---|
| Purple LED Module | 3V3 | yes | Single LED |
| Common Cathode RGB Module | 3V3 | yes | 3-colour LED |
| Traffic Lights Module | 3V3 | yes | Red/yellow/green LEDs |
| Active Buzzer | 3V3 | yes | Beeper (on/off) |
| Speaker Module | 3V3 | yes | Tone output via PWM (best in-kit sound) |
| Button Module | 3V3 | yes | Pushbutton |
| Tilt Sensor | 3V3 | yes | Tilt switch (digital) |
| PIR Motion Sensor | 3V3 | yes | Detects movement |
| Obstacle Avoidance Sensor | 3V3 | yes | IR proximity (digital) |
| 6812 RGB Module | 3V3* | yes | Addressable RGB (WS2812-type) |
| NTC-MF52AT Temp Sensor | 3V3 | med | Analog thermistor |
| Photoresistor | 3V3 | yes | Light level (analog) |
| Sound Sensor | 3V3 | yes | Noise/clap detect |
| Rotary Potentiometer | 3V3 | yes | Analog knob |
| IR Receiver | 3V3 | yes | Decodes the remote |
| Reed Switch Sensor | 3V3 | yes | Magnet detect (digital) |
| Rotary Encoder Module | 3V3 | med | Steps + direction + press |
| Joystick Module | 3V3 | yes | 2-axis + button |
| HT16K33 8×8 Dot Matrix | 3V3 | med | I²C LED matrix |
| TM1650 4-Digit Tube Display | 3V3 | med | 4-digit numeric display |
| Thin-film Pressure Sensor | 3V3 | yes | Press force (analog) |
| DS1307 Clock (RTC) | 5V | med | Real-time clock (I²C). Power 5V, I²C lines fine at 3.3V |
| SR01 Ultrasonic Sensor | **5V** | yes | Distance. **Echo line needs a divider to 3.3V** |
| 9G 90° Servo | **5V** | yes | Small servo. Power from 5V rail, signal from GPIO |
| Capacitive Touch Module | 3V3 | yes | Touch pad (digital) |
| Photo Interrupter Module | 3V3 | yes | Slot beam-break (digital) |
| Hall Sensor | 3V3 | yes | Magnetic field (analog/digital) |
| Flame Sensor | 3V3 | yes | IR flame detect |
| Line Tracking Sensor | 3V3 | yes | Black/white line (digital) |
| Analog Gas Sensor | **5V** | med | Gas level. **Analog out can exceed 3.3V — divide it** |
| XHT11 Temperature Sensor | 3V3 | yes | Temp + humidity (DHT11-compatible) |
| 18B20 Temperature Sensor | 3V3 | yes | Digital temp (DS18B20, 1-Wire) |
| 130 Motor | **5V** | med | DC motor. **Needs a driver or relay — never direct to GPIO** |
| Fan | **5V** | med | Motor + blade. Same rule as the motor |
| Collision Sensor | 3V3 | yes | Bump switch (digital) |
| Alcohol Sensor | **5V** | med | Like gas — **divide the analog out** |
| LCD_128X32_DOT Module | 3V3 | med | Small graphic LCD (I²C, Keyestudio lib) |
| 5-Channel AD Button Module | 3V3 | yes | 5 buttons on one analog pin |
| ADXL345 Acceleration Sensor | 3V3 | med | 3-axis accelerometer (I²C) |
| Laser Module | 3V3 | yes | Laser dot (on/off). **Do not point at eyes** |
| Steam Sensor | **5V** | yes | Water/steam (analog) — **divide the out** |
| RFID Reader Module | 3V3 | med | RC522-type reader. **3.3V only — never power at 5V** |
| 5V Relay Module | **5V** | med | Switches a load on/off. Trigger from GPIO |
| JMFP-4 17-Key Remote | n/a | yes | IR remote (pairs with the IR receiver) |
| White Card + RFID Key Fob | n/a | yes | RFID tags for the reader |
| ESP32-IO Expansion Board | both | yes | Seats the classic ESP32, breaks every GPIO to a keyed 3-pin header. See below. |
| 6-Slot AA Battery Holder | n/a | n/a | Powers the expansion board's separate rail (alkaline AA) |

\* The 6812 RGB data line runs at 3.3V logic fine for the small on-board count. Power from 5V for
best colour; 3.3V works for a handful of pixels.

## The expansion board is your friend (use it for beginners)

The Keyestudio **ESP32-IO Expansion Board** (one per kit, two available) seats the classic ESP32
and breaks every GPIO out to a **keyed 3-pin header (Signal / V / Ground)** — which kills the most
common beginner mistake, reversed power and ground. It also gives a **separate, switchable 3.3V/5V
rail** (jumper-selected) for the sensor headers when powered from the **6-slot AA holder** or a DC
input. That separate 5V rail is exactly how you run the **servo, 130 motor, fan and 5V relay**
without browning out the laptop USB. **Default any team using 5V actuators onto an expansion board +
AA pack.**

## Extra parts from the receipt

| Part | Qty | Use tonight |
|---|---|---|
| Elegoo HC-SR04 Ultrasonic | 1 | Same as SR01. **5V, divide the Echo.** |
| Elegoo 120-pc DuPont jumpers | 3 packs | Main wiring supply. Plenty. |
| Keyestudio ESP32 Plus | 5 | Classic pool, primary boards. |

## Set aside tonight (not part of the plug-and-play flow)

These need soldering, a breadboard, or hardware that isn't in the kit:

- **Freenove ESP32-CAM kit** — may need header soldering.
- **Lonely Binary 120-pc tactile buttons** & **12-pack bare 10K pots** — need a breadboard.
- **3W 4-ohm mini loudspeakers** — **need an amplifier (not in the kit)**. Use the kit **Speaker
  Module** or **Active Buzzer** for sound instead.
- **A-DIGISHUO USB-to-ESP-01 adapter** — not relevant to tonight's boards.
- **2-pack ESP32-C3 dev boards** — optional boards only.

## What this kit does and doesn't cover

The kit is a rich set of **3.3V-safe digital/analog/I²C sensors and small actuators**. It does
**not** contain the exotic parts the [flagship projects](../projects/) reach for — no thermal
camera, mmWave radar, LoRa, GPS, spectral or bio sensors. See
[`../hardware/README.md`](../hardware/README.md) for the full kit-vs-encyclopedia map.
