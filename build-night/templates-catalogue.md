# Templates catalogue — the menu

> The engine of the night. A team grabs a part, you point at its row, they hand the **library name +
> part name + the safe GPIO** to Claude Code. Pins shown are the Keyestudio defaults for the
> **classic** board and are known-good there. On the **S3**, apply the
> [S3 safe-pin map](safety-and-gpio.md) (no GPIO22 — remap I²C). **Flag the 5V rows out loud.**
> Distilled from the [facilitator pack](../MAY29th_README_PhysicalAIBuilders.md) §5.

> **Two anchor sources** for nearly everything below: the **Keyestudio wiki/GitHub** for the 42-in-1
> ESP32 Sensor Kit (KS5003/KS5004) — per-lesson Arduino code and the exact bundled libraries — and
> **Random Nerd Tutorials**, whose ESP32 sensor guides give tested sketches and wiring. Install
> Keyestudio's "Libraries" zip on each machine so the kit-specific headers resolve.

## A. Basic GPIO (start every team here for the first win)

| Project | Part | Library | Pins / notes |
|---|---|---|---|
| LED blink | Purple LED Module | none (`digitalWrite`) | any safe output GPIO |
| Fade (PWM) | Purple LED | none (`ledcWrite`/`analogWrite`) | any PWM-capable GPIO |
| RGB colour mix | Common Cathode RGB | none (3× PWM) | 3 safe GPIOs |
| Traffic lights | Traffic Lights Module | none | 3 safe GPIOs |
| Button toggles LED | Button Module | none (`INPUT_PULLUP`) | any safe GPIO |

**Make every team flash Blink first.** It proves board + driver + port + flash all work before any
sensor is blamed.

## B. Sensors

| Sensor (kit name) | Volt | Library (install this) | Pins / caveat |
|---|---|---|---|
| XHT11 Temp/Humidity | 3V3 | Keyestudio `xht11.h`, or Adafruit `DHT sensor library` + `Adafruit Unified Sensor` (treat as DHT11) | single data wire to a GPIO |
| 18B20 Temperature | 3V3 | `OneWire` + `DallasTemperature` (or Keyestudio `DS18B20.h`) | 1-Wire data, 4.7k pull-up if not a module |
| NTC-MF52AT thermistor | 3V3 | none (`analogRead` + Steinhart-Hart) | ADC1 (32–39 classic / 1–10 S3) |
| SR01 / HC-SR04 ultrasonic | **5V** | none (`pulseIn`) or `NewPing` | Trig from GPIO, **Echo through 1k+2k divider** |
| PIR Motion | 3V3 | none (`digitalRead`, use an interrupt) | any input GPIO |
| Photoresistor | 3V3 | none (`analogRead`) | ADC1 pin |
| Sound Sensor | 3V3 | none (`analogRead`/`digitalRead`) | analog on ADC1 |
| IR Receiver | 3V3 | `IRremote` | any input GPIO |
| Flame / Hall / Reed / Tilt / Collision / Obstacle / Line / Photo-interrupter | 3V3 | none | digital or ADC1 |
| Analog Gas / Alcohol / Steam | **5V** | none (`analogRead`) | **divide the analog out to 3.3V**; gas needs warm-up |
| Thin-film Pressure | 3V3 | none (`analogRead`) | ADC1 pin |
| Capacitive Touch | 3V3 | none (digital), or built-in `touchRead` | digital out |
| ADXL345 accelerometer | 3V3 | `Adafruit_ADXL345` + `Adafruit Unified Sensor` | I²C: classic SDA21/SCL22, **S3 remap 8/9** |

## C. Displays

| Display | Volt | Library | Pins / caveat |
|---|---|---|---|
| LCD_128x32_DOT | 3V3 | Keyestudio `lcd128_32_io.h` (kit lib) | I²C, S3 remap pins |
| HT16K33 8×8 Dot Matrix | 3V3 | `Adafruit LED Backpack` + `Adafruit GFX` | I²C addr 0x70, S3 remap pins |
| TM1650 4-Digit Tube | 3V3 | Keyestudio `TM1650.h` (kit lib) | 2-wire, kit lib needed |

## D. Actuators & outputs

| Actuator | Volt | Library | Pins / caveat |
|---|---|---|---|
| 9G Servo | **5V** | `ESP32Servo` | power 5V from expansion/AA, signal from GPIO, common ground |
| 130 Motor / Fan | **5V** | none | **switch via the 5V relay — never direct to a GPIO** |
| Active Buzzer | 3V3 | none (`digitalWrite`) | any safe GPIO |
| Speaker Module | 3V3 | none (`tone()`/`ledc`) | PWM GPIO. Best sound option in the kit |
| 5V Relay | **5V** | none (`digitalWrite` to IN) | coil 5V, trigger from GPIO. Switch the motor/fan/laser, **avoid mains** |
| Laser Module | 3V3 | none (on/off like an LED) | any safe GPIO. **Do not point at eyes** |
| 6812 RGB | 3V3* | `Adafruit NeoPixel` (or `FastLED`) | data from GPIO, `NEO_GRB + NEO_KHZ800` |

## E. Input devices

| Device | Volt | Library | Pins / caveat |
|---|---|---|---|
| Rotary Potentiometer | 3V3 | none (`analogRead`) | ADC1 pin |
| Rotary Encoder | 3V3 | none (poll CLK/DT/SW) or `AiEsp32RotaryEncoder` | SW uses `INPUT_PULLUP`, avoid strapping pins |
| Joystick | 3V3 | none (2× `analogRead` + `digitalRead`) | two ADC1 pins + one digital |
| 5-Channel AD Button | 3V3 | none (`analogRead`, compare thresholds) | one ADC1 pin |

## F. RFID, timekeeping, IR remote

| Project | Volt | Library | Pins / caveat |
|---|---|---|---|
| RFID read a card UID | 3V3 | `MFRC522` (SPI) or `MFRC522_I2C` | **RC522 is 3.3V only, never 5V.** Print the UID from the White Card / Key Fob |
| DS1307 real-time clock | 5V (power) | `RTClib` (Adafruit) or `RtcDS1307` (Makuna) | I²C addr 0x68, power 5V, I²C lines 3.3V fine, S3 remap pins |
| IR remote decode | 3V3 | `IRremote` | JMFP-4 remote + IR Receiver, decode NEC codes |

## G. Connectivity (the Physical AI / IoT payoff)

| Capability | Library | Notes |
|---|---|---|
| Wi-Fi basics (STA/AP) | `WiFi.h` (built-in) | both boards |
| Web server (phone page of readings/buttons) | `WiFi.h` + `ESPAsyncWebServer` + `AsyncTCP` | open the ESP32's IP on a phone on the same Wi-Fi — great live demo |
| Bluetooth Classic | `BluetoothSerial.h` (built-in) | **classic ESP32 only** — the S3 does **not** do BT Classic |
| BLE (both, the S3's strength) | `BLEDevice.h` (built-in) / Nordic UART | the `anthropics/claude-desktop-buddy` pattern lives here |
| MQTT to a dashboard | `PubSubClient` or `Adafruit MQTT` | publish readings to a broker / Adafruit IO |

## Crowd-pleasing combos → tonight's buildable projects

Each combo below is a **full kit-only project guide** in `projects/`:

| Combo | Parts | Guide |
|---|---|---|
| Parking sensor (beeps faster as you approach) | SR01 ultrasonic + Active Buzzer | [`kit-parking-sensor`](../projects/kit-parking-sensor/) |
| Sound-reactive lights | Sound Sensor + 6812 RGB | [`kit-sound-reactive-lights`](../projects/kit-sound-reactive-lights/) |
| Tripwire / intruder alert | PIR + 5V Relay + Laser (or Buzzer) | [`kit-tripwire-alert`](../projects/kit-tripwire-alert/) |
| Weather station | XHT11 + LCD_128x32 (or HT16K33) | [`kit-weather-station`](../projects/kit-weather-station/) |
| Mini game | Joystick + HT16K33 8×8 dot matrix | [`kit-joystick-arcade`](../projects/kit-joystick-arcade/) |
| Access control | RFID Reader + 9G Servo + Buzzer | [`kit-rfid-doorman`](../projects/kit-rfid-doorman/) |

## How to use this catalogue on the night

1. Identify the part, jump to its row.
2. Read the team the **library name** — that is literally what they tell Claude Code to install.
3. Give Claude the trio: **board** (`esp32` or `esp32s3`) + **part name** + **library** + the
   **pins** from the row.
4. For any **5V** row, route power through the **expansion board + AA pack**, not the laptop USB.
5. Combine a sensor row with a display or actuator row for the "wow".
