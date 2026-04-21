# Motion & inertial sensors

Accelerometers, gyroscopes, magnetometers, vibration. The "is it moving, and how?" stack.

## Part catalog

| Part | Axes | Interface | Note |
|---|---|---|---|
| **MPU6050** | 6-DOF (accel+gyro) | I²C | Classic cheap IMU; thermal drift — recalibrate often. |
| **MPU9250** | 9-DOF | I²C / SPI | Adds magnetometer; discontinued but still in stock everywhere. |
| **ICM-20948** | 9-DOF | I²C / SPI | Modern replacement for MPU9250; DMP on-chip fusion. |
| **LSM6DSOX / LSM6DSV** | 6-DOF | I²C / SPI | ST, programmable FSM for gesture on-chip, low power. |
| **BMI270** | 6-DOF | I²C / SPI | Bosch's modern choice; wake-on-gesture built-in. |
| **BNO055** | 9-DOF + on-chip fusion | I²C | Outputs quaternions directly — wonderful for the lazy. |
| **BNO085 / BNO086** | 9-DOF + fusion | I²C / SPI / UART | CEVA SH-2; "stable, calibrated, understandable" output. |
| **ADXL345 / ADXL355** | 3-axis accel | I²C / SPI | Low-noise versions useful for seismology / structural health. |
| **Piezo disk (~20 mm)** | Vibration | Analog | $0.50; surprisingly powerful for machine-health audio. |
| **KY-003 / A3144 Hall** | Magnetic presence | Digital | Door-open, rotation counts, simple positional feedback. |
| **AS5600** | Magnetic rotary encoder | I²C | 12-bit absolute angle from a cheap diametric magnet. |

## Interface notes

- Most IMUs support both I²C and SPI. Prefer SPI for high-rate (>1 kHz) streaming.
- I²C at high ODR is fine for quaternions but watch for bus collisions if you share with other sensors.
- Magnetometers are very sensitive to local DC magnetic fields — keep >3 cm from speakers, motors, LiPo packs.

## Capability contribution

- **Transform**: accel → orientation (Madgwick / Mahony / on-chip DMP).
- **Find**: tap / fall / free-fall / peak vibration → event.
- **Distill**: step count, roll summary per hour, RMS vibration per minute.
- **Reason**: classify activity (sitting / walking / cycling / sleeping) with a TinyML model.
- **Do**: close the loop to actuators (self-balancing, gimbal, haptic feedback).

## Pairs with (asymmetry combos)

- **IMU + GPS + BMP388 (baro)** — full inertial navigation; dead-reckoning through GPS dropout (canyons, tunnels, tree cover, surf).
- **IMU + microphone** — machine-health monitoring: vibration spectrum + acoustic signature classify a bearing fault.
- **IMU + NeoPixel** — a badge that lights when you're too still too long (anti-DVT reminder).
- **IMU + EMG (MyoWare)** — gesture + intent, for prosthetic / XR controllers.
- **Multi-IMU mesh (torso + wrist + ankle)** — full-body pose via ESP-NOW sync; a home-grown motion capture.

## Where to buy

- **DigiKey / Mouser** for Bosch / ST / TDK genuine.
- **Adafruit / Pimoroni / SparkFun** for nice breakout boards with pull-ups in place.
- **Watterott / Pimoroni** for BNO085 breakouts.

## Gotchas

- **Gyro drift** is real; re-bias on detected stillness (below a small accel-variance threshold).
- **Magnetometer calibration** (hard-iron + soft-iron) is mandatory for reliable heading. Spin the device in a figure-8 during init.
- **High-G shocks** can latch some IMUs into a bad state — use the reset line in firmware, not just via power-cycle.
- **USB/Wi-Fi EMI** can bleed into accelerometer analog paths; keep the IMU away from the antenna and decoupling capacitors.
