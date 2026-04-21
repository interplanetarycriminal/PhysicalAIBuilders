# Actuators (the Do side)

Motors, servos, solenoids, relays, speakers, haptic drivers, IR blasters, thermal printers. See [`../../capabilities/do.md`](../../capabilities/do.md) for safety rules.

## Part catalog

| Part | Kind | Interface | Note |
|---|---|---|---|
| **SG90 / MG996R / DS-3218** | Hobby servo | PWM | Cheap to strong; check stall torque. |
| **Dynamixel AX-12 / XL-330** | Smart serial servo | Half-duplex UART | Position feedback, torque control. |
| **28BYJ-48 + ULN2003** | Stepper | GPIO | Tiny stepper; accurate, slow. |
| **NEMA 17 + TMC2209** | Stepper | Step/Dir + UART | CNC / 3D-printer-grade silent stepper. |
| **N20 DC gearmotor + TB6612 / DRV8833** | DC motor | PWM | Classic robotics. |
| **BLDC + VESC / SimpleFOC** | Brushless motor | UART / I²C | Precise field-oriented control; pair with AS5600 encoder. |
| **5 V / 12 V solenoid + MOSFET + flyback diode** | Linear push/pull | GPIO→MOSFET | Valves, latches, kick drums. |
| **SSR (solid-state relay)** | Mains switching | GPIO (isolated) | Kiln / heater / appliance control; zero-cross types for clean switching. |
| **Mechanical relay (SRD-05VDC)** | General switching | GPIO (isolated) | Cheap; clicks; good for DC loads. |
| **MAX98357A** | I²S mono DAC+amp | I²S | Speech / music out. |
| **PCM5102 + amp** | I²S stereo DAC | I²S | Higher fidelity audio. |
| **DRV2605** | Haptic driver | I²C | Library of haptic effects; pairs with LRA motors. |
| **IR LED + transistor** | IR blaster | GPIO (via timer) | Control TVs / ACs / cameras. |
| **CSN-A2 / similar 58 mm thermal printer** | Receipt print | UART | "The machine gave me a thing" UX. |
| **Peltier + N-channel MOSFET** | Heat/cool | PWM | Caveat: inefficient; needs heatsink. |
| **ER / stepper-driven solenoid valve** | Water / gas | Relay | Garden, lab, brewing. |
| **DMX over MAX485** | Stage lights | UART→RS-485 | Control professional lighting fixtures. |

## Interface notes

- **Use a separate power rail** for motors / heaters. A common MCU+motor rail will brown out the ESP32.
- **Flyback diodes** on every inductive load, always.
- **PWM frequency selection** matters — >20 kHz for DC motors so you don't hear whine; match servo expectation (50 Hz) precisely.

## Capability contribution

- Pure **Do**. These are the output-side of every asymmetry loop.

## Pairs with (asymmetry combos)

- **Servo + VL53L5CX + LLM** — a robot arm that points at the thing you just described in speech.
- **Solenoid + piezo + mic** — a percussive instrument that listens, then plays a rhythm on top.
- **DMX + mic + mmWave** — DJ-reactive stage lighting that also respects a dance-floor occupancy map.
- **Thermal printer + LLM** — a device that prints you a haiku about your day, seeded from your sensor history.

## Where to buy

- **Pololu** for excellent motor drivers and small motors.
- **ODrive / VESC** direct for BLDC driver boards.
- **Adafruit / SparkFun / Pimoroni** for breakouts.
- **AliExpress** for bulk servos, steppers, MOSFET boards (verify current handling).
- **DigiKey / Mouser** for TMC, TI motor drivers.

## Gotchas

- **Mains electricity** is not a DIY hobby subject. Use SSRs, enclosures, fused supplies; learn from a qualified electrician if wiring 110/240 V.
- **Servo jitter** comes from the ESP32 non-hardware-PWM; use the LEDC/MCPWM peripheral.
- **Speaker pop** on amplifier power-up — mute the amp before enabling.
- **Heating elements** need thermal fusing, not just firmware watchdogs.
