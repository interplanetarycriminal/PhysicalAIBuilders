# Physical AI Builders — ESP32 + Claude Code Build Night

**Facilitator Pack (Brisbane).** A pick-up-and-go kit for a 2.5 hour, hands-on build night. Teams of two work on their own laptops, plug an ESP32 into USB, grab a supplied sensor, and use **Perplexity + Claude Code** to ship a working proof-of-concept fast. No soldering. This document is self-contained: hand it out as-is.

> **The One Rule, read it aloud first:** ESP32 GPIO pins are **3.3V logic and are NOT 5V tolerant.** 5V parts (ultrasonic, relay, motor, fan, gas/alcohol/steam sensors) get power from the 5V rail, and their **signal never goes straight into a GPIO.** When in doubt, power the sensor from 3.3V.

---

## Contents
1. [Inventory](#1-inventory)
2. [Safety](#2-safety)
3. [Workflow: the build loop](#3-workflow-the-build-loop)
4. [Bottlenecks: momentum and getting unstuck](#4-bottlenecks-momentum-and-getting-unstuck)
5. [Templates: the project catalogue](#5-templates-the-project-catalogue)
6. [Run-of-show, escalation, playbook](#6-run-of-show-escalation-playbook)
7. [Caveats and assumptions](#7-caveats-and-assumptions)
8. [Sources](#8-sources)

---

## 1. Inventory

What is on the night, grouped so you can see at a glance what a team can grab. Voltage column: **3V3** means the module runs and signals safely at 3.3V (GPIO-safe), **5V** means it needs 5V power and you must be careful with its signal line. "Easy" flags the fast, beginner-friendly first wins.

### Boards (10 flashable, all plug-and-play with DuPont cables)
| Board | Qty | USB | Notes |
|---|---|---|---|
| **ESP32-WROOM-32E** (classic dual-core) | 5 | USB-to-UART (CP2102/CH340) | The default board. All Keyestudio kit code is written for this. Easiest path. |
| **ESP32-S3-WROOM-1** | 5 | Native USB (USB-OTG/Serial-JTAG) | Use for BLE, native-USB, and "AI agent reacts physically" demos. Different pinout, see Safety. |
| ESP32-C3 mini (2-pack) | 2 | varies | Present but not the focus. Fewer GPIOs, different pinout. Optional only. |
| Keyestudio ESP32 Plus | 5 | CP2102 | Same family as the WROOM-32E, treat identically. (These are the receipt boards; the "classic" pool.) |
| Freenove ESP32-CAM kit | 1 | CH340 | **Off-limits unless pre-assembled.** May need header soldering. Set aside. |

> You effectively have **two pools**: a large **classic ESP32** pool (WROOM-32E + Keyestudio Plus) and a **5-strong S3** pool. Default everyone to classic for the first win.

### Sensors and modules (you have ~2 of each: two identical Keyestudio 42-in-1 kits, model KS5003)
| Module (kit name) | Volt | Easy | What it is |
|---|---|---|---|
| Purple LED Module | 3V3 | yes | Single LED |
| Common Cathode RGB Module | 3V3 | yes | 3-colour LED |
| Traffic Lights Module | 3V3 | yes | Red/yellow/green LEDs |
| Active Buzzer | 3V3 | yes | Beeper (on/off) |
| Speaker Module | 3V3 | yes | Tone output via PWM |
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
| HT16K33 8x8 Dot Matrix | 3V3 | med | I2C LED matrix |
| TM1650 4-Digit Tube Display | 3V3 | med | 4-digit numeric display |
| Thin-film Pressure Sensor | 3V3 | yes | Press force (analog) |
| DS1307 Clock (RTC) | 5V | med | Real-time clock (I2C). Power 5V, I2C lines fine at 3.3V |
| SR01 Ultrasonic Sensor | **5V** | yes | Distance. **Echo line needs divider to 3.3V** |
| 9G 90° Servo | **5V** | yes | Small servo. Power from 5V rail, signal from GPIO |
| Capacitive Touch Module | 3V3 | yes | Touch pad (digital) |
| Photo Interrupter Module | 3V3 | yes | Slot beam-break (digital) |
| Hall Sensor | 3V3 | yes | Magnetic field (analog/digital) |
| Flame Sensor | 3V3 | yes | IR flame detect |
| Line Tracking Sensor | 3V3 | yes | Black/white line (digital) |
| Analog Gas Sensor | **5V** | med | Gas level. **Analog out can exceed 3.3V, divide it** |
| XHT11 Temperature Sensor | 3V3 | yes | Temp + humidity (DHT11-compatible) |
| 18B20 Temperature Sensor | 3V3 | yes | Digital temp (DS18B20, 1-Wire) |
| 130 Motor | **5V** | med | DC motor. **Needs driver or relay, never direct to GPIO** |
| Fan | **5V** | med | Motor + blade. Same rule as the motor |
| Collision Sensor | 3V3 | yes | Bump switch (digital) |
| Alcohol Sensor | **5V** | med | Like gas, **divide the analog out** |
| LCD_128X32_DOT Module | 3V3 | med | Small graphic LCD (I2C, Keyestudio lib) |
| 5-Channel AD Button Module | 3V3 | yes | 5 buttons on one analog pin |
| ADXL345 Acceleration Sensor | 3V3 | med | 3-axis accelerometer (I2C) |
| Laser Module | 3V3 | yes | Laser dot (on/off). Do not point at eyes |
| Steam Sensor | **5V** | yes | Water/steam (analog), **divide the out** |
| RFID Reader Module | 3V3 | med | RC522-type reader. **3.3V only, do not power at 5V** |
| 5V Relay Module | **5V** | med | Switches a load on/off. Trigger from GPIO |
| JMFP-4 17-Key Remote | n/a | yes | IR remote (pairs with IR receiver) |
| White Card + RFID Key Fob | n/a | yes | RFID tags for the reader |
| ESP32-IO Expansion Board | both | yes | See note below. Makes wiring beginner-safe |
| 6-Slot AA Battery Holder | n/a | n/a | Powers the expansion board (alkaline AA) |

\* 6812 RGB runs its data line at 3.3V logic fine for the small on-board count. Power from 5V if available for best colour, 3.3V works for a handful of pixels.

### Extra parts from the receipt
| Part | Qty | Use tonight |
|---|---|---|
| Elegoo HC-SR04 Ultrasonic | 1 | Same as SR01. **5V, divide the Echo.** |
| Lonely Binary 120-pc Tactile Buttons | 1 set | Needs a breadboard. Set aside for fast POC |
| 2pcs ESP32-C3 Dev Board | 1 pack | Optional boards only |
| Keyestudio ESP32 Plus | 5 | Classic pool, primary boards |
| Freenove ESP32-CAM kit | 1 | Off-limits unless pre-assembled |
| Elegoo 120pc DuPont jumpers | 3 packs | Main wiring supply. Plenty |
| Lonely Binary 12-pack 10K pots | 1 pack | Bare pots, need a breadboard. Set aside |
| 3W 4-ohm Mini Loudspeakers | 2 packs | **Need an amplifier (not in kit).** Use the kit Speaker Module/buzzer for sound instead |
| A-DIGISHUO USB-to-ESP-01 adapter | 1 pack | Not relevant to tonight's boards. Set aside |

### Cables and accessories
Two kit USB cables, three 120-piece DuPont jumper packs (M-M, M-F, F-F as supplied), kit F-F DuPont wire, and the two ESP32-IO expansion boards. You are not short on wire.

### Use the ESP32-IO Expansion Board for beginners
The Keyestudio expansion board (one per kit, so two available) is your friend. It seats the classic ESP32 and breaks every GPIO out to a **keyed 3-pin header (Signal / V / Ground)**, which kills the most common beginner mistake: reversed power and ground. It also provides a **separate, switchable power rail (3.3V or 5V via a jumper)** for the sensor headers when you power it from the **6-slot AA holder** or a DC input. That separate 5V rail is exactly how you run the **servo, 130 motor, fan, and 5V relay** without browning out the laptop USB. Default any team using 5V actuators onto an expansion board + AA pack.

---

## 2. Safety

Print this section as a one-pager. Plain language, scannable.

### The 5 rules to read aloud before anyone plugs in
1. **3.3V is the law.** ESP32 GPIO runs at 3.3V and is not 5V tolerant. Putting 5V into a GPIO can degrade or kill the pin and sometimes the chip. Power sensors from 3.3V unless the part needs 5V, and if it does, keep its signal out of the GPIO or drop it with a divider.
2. **Never plug or unplug parts while powered.** Pull the USB cable first, wire it, then power up. Hot-plugging is the classic way to short two pins and fry something.
3. **Check polarity before power.** V to V (3V3 or 5V), G to GND, Signal to the GPIO you chose. Reversing V and G destroys a module in seconds. The expansion board's keyed headers prevent this.
4. **One disconnect reflex:** if anything smells hot, gets too hot to hold, or smokes, **pull the USB cable and the AA pack immediately**, then investigate. Do not wait and see.
5. **Handle boards by the edges.** Fingers on the PCB edge, not on the chips, pins, or gold contacts.

### Safe GPIO map: ESP32-WROOM-32E (the classic default board)
- **3.3V logic, not 5V tolerant.**
- **Input-only pins (no output, no internal pull-up):** GPIO **34, 35, 36, 39**. Great for analog sensors, useless for LEDs/relays.
- **Strapping pins (affect boot, avoid for sensors if a board won't flash):** GPIO **0, 2, 5, 12, 15**.
- **Do not use at all (wired to internal flash):** GPIO **6, 7, 8, 9, 10, 11**.
- **Analog with WiFi on:** use **ADC1 pins = GPIO 32 to 39**. The ADC2 pins (including 0, 2, 4, 12 to 15, 25 to 27) stop working when WiFi is active, which surprises everyone.
- **Safe general-purpose pins (output, PWM, I2C, etc.):** GPIO **4, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27**, plus 32/33 for analog or output.
- **Default I2C:** SDA = **21**, SCL = **22**.

### Safe GPIO map: ESP32-S3-WROOM-1 (the BLE / native-USB board)
- **3.3V logic, not 5V tolerant.**
- **GPIO range:** 0 to 21 and 26 to 48. **Note GPIO 22, 23, 24, 25 do not exist on the S3.** This is the big gotcha: any classic-ESP32 example that uses **GPIO22 (e.g. I2C SCL) will not work on the S3.** Remap I2C on the S3, for example `Wire.begin(8, 9)` (SDA=8, SCL=9), or pick any valid free pair.
- **Native USB pins (leave alone):** GPIO **19 (D-)** and **20 (D+)**.
- **Strapping pins:** GPIO **0, 3, 45, 46**.
- **Do not use (internal flash, and PSRAM on PSRAM modules):** GPIO **26 to 32**, and on Octal-PSRAM variants (the common N8R8) also **33 to 37**. If an S3 sketch fails only on those high pins, that is why. Move to GPIO **4 to 18** or **38 to 42**.
- **Analog with WiFi on:** use **ADC1 = GPIO 1 to 10**. ADC2 (GPIO 11 to 20) conflicts with WiFi, same as the classic.
- **Serial over USB:** you must enable **USB CDC On Boot** (see Workflow) or the serial monitor stays blank.

### Voltage and power (the stuff that fries boards)
- **Which kit parts are 5V:** SR01/HC-SR04 ultrasonic, 5V relay, 130 motor, fan, analog gas, alcohol, steam, and DS1307 (power). For the analog 5V sensors, their output can swing above 3.3V, so **put a divider (1k + 2k) on the signal** before it reaches a GPIO, or read them through the expansion board's logic-safe header. For the ultrasonic, the **Echo line specifically needs a divider**; Trig is fine straight from a GPIO.
- **USB current limit and brownout:** an ESP32 can spike to 400 to 500mA when WiFi starts. Many laptop ports, hubs, and keyboard passthroughs supply less. Symptoms of starvation are random resets, `rst:0xf (BROWNOUT_RST)` in the serial monitor, and failed uploads. Fix with a short, thick, known-good **data** cable (not charge-only), a direct laptop port or powered hub, and by powering servos/motors/relays/LED strips from the **expansion board + AA pack**, not the 3.3V pin.
- **Do not feed more than 5V into the 5V/VIN pin**, and never feed voltage into the 3.3V pin unless it is a regulated 3.3V source (that bypasses the board's protection).

### Shorts, wiring, breadboards, DuPont
- No bare wires touching. A loose jumper bridging V and GND is a dead short.
- Push DuPont connectors fully onto pins. Half-seated wires give intermittent faults that look like broken code.
- One change at a time, with power off, then re-power. Keeps debugging sane.

### ESD (static), kept light but real
Static you cannot even feel can damage or quietly weaken a chip. Touch a grounded metal object (the laptop chassis) before handling bare boards, keep parts in their anti-static bags until needed, and do not slide boards across synthetic surfaces. Brisbane's humidity lowers static risk most of the year, but air-conditioning dries the air, so keep the habit.

### Heat
A regulator running mildly warm is normal. Too hot to keep a finger on, or rapidly heating, means unplug now. Usual causes: reversed polarity, a short, a stalled servo, a motor wired straight to a pin, or a sensor on the wrong rail. A servo that buzzes and gets hot is usually jammed against a stop, cut power and free it.

### Battery safety (6-slot AA holder)
The kit uses **alkaline AA** (not lithium), which is low-risk, but still: observe polarity, do not short the terminals, do not mix old and new cells, and remove the pack if it gets warm. Use it to power the expansion board's separate rail, which is the clean way to run 5V actuators.

### If something goes wrong, quick triage
1. Pull the USB cable and the AA pack. 2. Look for scorch marks, smell, a hot part. 3. Check polarity and shorts before re-powering. 4. Re-power after one fix at a time. 5. If a board is confirmed dead, swap it, do not burn session time on a resurrection.

---

## 3. Workflow: the build loop

One simple loop, repeated all night. Default toolchain is the **Arduino framework** (Arduino IDE or PlatformIO), the fastest "plug in, paste, flash, see it work" path. It adapts cleanly if a team picks ESP-IDF or MicroPython.

### The loop in one line
**Idea → Perplexity scopes it → Claude Code builds and flashes it → test on hardware → paste the exact error back → iterate → working POC.**

### Step by step
1. **Idea (5 min, time-boxed).** Smallest demonstrable thing the grabbed sensor can do. "Make the buzzer beep when the ultrasonic sees something close." Not "build a security system."
2. **Perplexity scopes hardware, library, wiring.** Confirm which library, how it wires to your specific board (3.3V vs 5V), and the minimal working pattern.
3. **Hand findings to Claude Code.** Give it the board, the part, the goal, the pins, and the library name. Ask it to state its plan first, then generate the sketch.
4. **Claude Code builds and flashes.** As a CLI agent it can run the compile and upload commands and read the serial output back.
5. **Test on the hardware.** Watch the board and the serial monitor.
6. **Observe the failure.** Compile error, garbage reading, board resetting. Note exactly what happened.
7. **Paste the exact error back to Claude Code.** The full verbatim compiler or serial output. This is the highest-leverage move in the loop.
8. **Iterate.** Repeat 4 to 7 in minutes.
9. **Working POC.** Stop when it does the one thing. Then consider one stretch feature.

### What makes a good Perplexity scoping query
Specificity wins. Use the vocabulary a good tutorial would use, name the exact board and part, and pin it to 2026 to avoid stale answers.
- Weak: "how to use a temperature sensor"
- Strong: *"Wire an XHT11 (DHT11-compatible) temp/humidity sensor to an ESP32-WROOM-32E using the Arduino framework. Which library and exact GPIO? Give a minimal sketch and cite sources. As of 2026."*
- Strong (S3): *"I2C OLED/RTC on an ESP32-S3-WROOM-1. The S3 has no GPIO22, what SDA/SCL pins should I use and how do I set them in Wire.begin()? Minimal example."*
- Always ask for the **library name plus the voltage caveat** ("does this sensor's output need a divider for 3.3V?").

### What context to give Claude Code, every time
Treat Claude like an eager intern: fast and helpful, but it will not be error-free first try, so give it everything and check its work.
- **Exact board / FQBN.** Classic: `esp32:esp32:esp32` (Arduino IDE board "ESP32 Dev Module"). S3: `esp32:esp32:esp32s3` (board "ESP32S3 Dev Module").
- **Exact sensor / part number** and the **library to install** (prevents it inventing a function that does not exist).
- **What you are trying to do**, in one plain sentence.
- **The wiring / pin map you actually used** ("XHT11 data on GPIO 4, powered from 3.3V").
- **The exact error messages**, the whole compiler output or serial dump, not a summary.
- **Tell it to state its plan before editing** and to leave working code intact.

### Compile and flash commands
**arduino-cli:**
```
arduino-cli core install esp32:esp32
arduino-cli board list                       # find the port
arduino-cli compile --fqbn esp32:esp32:esp32 <sketch>      # classic
arduino-cli upload -p <PORT> --fqbn esp32:esp32:esp32 <sketch>
# S3 with serial-over-USB enabled:
arduino-cli compile --fqbn esp32:esp32:esp32s3:CDCOnBoot=cdc <sketch>
arduino-cli upload  -p <PORT> --fqbn esp32:esp32:esp32s3:CDCOnBoot=cdc <sketch>
```
**PlatformIO** (`platformio.ini` board IDs): classic = `esp32dev`, S3 = `esp32-s3-devkitc-1`, platform `espressif32`, framework `arduino`.
```
pio run                 # build
pio run -t upload       # flash
pio device monitor -b 115200
```

### Board-specific flashing gotchas (say these out loud at setup)
- **Classic WROOM-32E / Keyestudio Plus:** needs the **CP2102 (or CH340) USB driver** installed. If upload hangs on `Connecting... _ _ _`, **hold the BOOT button**, and release once the percentage starts.
- **ESP32-S3:** has **native USB**. In Arduino IDE set **USB CDC On Boot: Enabled** or the serial monitor stays blank. If upload fails, put it in download mode: **hold BOOT, tap RESET, release BOOT.** Devkits often expose two USB ports (one native USB, one UART), either flashes, pick whichever shows a port.
- Baud for the serial monitor is **115200**.

### The motivating, real example
In April 2026 Anthropic released the open-source, MIT-licensed `anthropics/claude-desktop-buddy`: an ESP32 "desk pet" (Arduino framework) that talks to Claude over **Bluetooth LE** (Nordic UART, newline-delimited JSON) and physically reacts to Claude Code's permission prompts, sleeping when idle, waking on a session, getting visibly impatient when an approval is waiting. It is a maker reference, not an official product feature, and a full port is more than one night. Use it as the **pattern** for tonight: AI scopes the idea, the agent builds it, the ESP32 reacts in the physical world. The S3 boards are the ones to try this on as a stretch goal.

---

## 4. Bottlenecks: momentum and getting unstuck

### Read this to the room
You will feel more capable at the end of the night than at the start. Guaranteed. The first small win (an LED blinks on command) rewires how the next problem feels. Momentum, not talent, carries a build night. So the goal of the first 30 minutes is not the cool project, it is the **first win, fast**. Get every team to "something visibly works" early and the room's energy does the rest. Same with the AI: Claude Code is an eager intern, not an oracle. It will get things wrong, you will correct it, and that back-and-forth is the skill, not a sign you are failing.

### The unsticking framework
Two kinds of stuck: **what to build** and **how to proceed.** Same toolkit.

**Stuck on what to build:**
1. **Let the sensor be the seed.** Whatever you grabbed is the idea.
2. **Pick the smallest demonstrable version.** "It prints the value to serial" is a complete, demo-able win.
3. **Brainstorm with the AI, time-boxed.** Ask Claude Code or Perplexity: *"I have an ESP32 and a [sensor]. Give me 5 dead-simple POC ideas I can build in under an hour."* Pick one in 3 minutes.
4. **Grab a known-working template and modify it.** Beats a blank page every time.

**Stuck on how to proceed:**
1. **Constrain the scope.** Cut the blocking feature. No WiFi yet? Serial-print instead.
2. **Shrink to the smallest failing piece.** Get the raw reading working alone before wiring a display, actuator, or network.
3. **Time-box the decision.** Debated an approach for more than 5 minutes? Pick one and try it. A tried wrong answer teaches faster than a perfect plan.
4. **Paste the exact error into Claude Code.** Most "how do I proceed" moments are one specific error.
5. **Fall back to a known-good template** (LED blink), confirm the toolchain is fine, then layer your change back one step at a time.
6. **Call the facilitator** for a 60-second sanity check (right port? right voltage? right library?).

### Sensor-as-seed, mapped to the actual kit
| Grab this part | Smallest first win (serial) | Obvious "wow" build |
|---|---|---|
| SR01 / HC-SR04 ultrasonic | Print distance in cm | Parking sensor: beep faster as you get closer (+ Active Buzzer) |
| PIR Motion | Print "motion!" | Presence alert / tripwire (+ relay + laser) |
| Joystick | Print X/Y/button | Game controller for the 8x8 dot matrix |
| HT16K33 8x8 Dot Matrix | Light one pixel | Scrolling message, or a tiny game |
| 9G Servo | Sweep 0 to 180 | Gauge needle driven by a sensor value |
| RFID Reader | Print a card's UID | Access badge: known card opens a servo "door" |
| Sound Sensor | Print noise level | Clap switch, or sound-reactive 6812 RGB lights |
| ADXL345 | Print X/Y/Z tilt | Motion-reactive 6812 RGB / spirit level on the LCD |
| XHT11 temp/humidity | Print temp and RH | Weather readout on the LCD_128x32 |
| 6812 RGB | One pixel turns red | Reacts to sound, motion, or distance |

### The one rule for the night
**Momentum beats perfection.** A scrappy thing that works beats an elegant thing that does not compile. Ship the smallest win, then build on it.

---

## 5. Templates: the project catalogue

The engine of the night. A team grabs a part, you point at its row, they hand the **library name + part name + the safe GPIO** to Claude Code. Pins shown are the Keyestudio defaults for the **classic** board and are known-good there. On the **S3**, apply the S3 safe-pin map (and remember there is no GPIO22, so remap I2C). Flag the 5V rows out loud.

> **Two anchor sources** for nearly everything below: the **Keyestudio wiki/docs and GitHub for the 42-in-1 ESP32 Sensor Kit (KS5003/KS5004)**, which ship per-lesson Arduino code and the exact bundled libraries; and **Random Nerd Tutorials**, whose ESP32 sensor guides give tested sketches and wiring for the common parts. Install Keyestudio's "Libraries" zip on each machine so the kit-specific headers resolve.

### A. Basic GPIO (start every team here for the first win)
| Project | Part | Library | Pins / notes | Code pointer |
|---|---|---|---|---|
| LED blink | Purple LED Module | none (`digitalWrite`) | any safe output GPIO | Arduino Examples > Basics > Blink; Keyestudio wiki Lesson 1 |
| Fade (PWM) | Purple LED | none (`ledcWrite`/`analogWrite`) | any PWM-capable GPIO | Keyestudio wiki |
| RGB colour mix | Common Cathode RGB | none (3x PWM) | 3 safe GPIOs | Keyestudio wiki |
| Traffic lights | Traffic Lights Module | none | 3 safe GPIOs | Keyestudio wiki |
| Button toggles LED | Button Module | none (`INPUT_PULLUP`) | any safe GPIO | Keyestudio wiki; RNT digital input |

**Make every team flash Blink first.** It proves board + driver + port + flash all work before any sensor is blamed.

### B. Sensors
| Sensor (kit name) | Volt | Library (install this) | Pins / caveat | Code pointer |
|---|---|---|---|---|
| XHT11 Temp/Humidity | 3V3 | Keyestudio `xht11.h`, or Adafruit `DHT sensor library` + `Adafruit Unified Sensor` (treat as DHT11) | single data wire to a GPIO | Keyestudio wiki; RNT "ESP32 DHT11" |
| 18B20 Temperature | 3V3 | `OneWire` + `DallasTemperature` (or Keyestudio `DS18B20.h`) | 1-Wire data, 4.7k pull-up if not a module | RNT "ESP32 DS18B20" |
| NTC-MF52AT thermistor | 3V3 | none (`analogRead` + Steinhart-Hart) | ADC1 pin (32 to 39 classic / 1 to 10 S3) | Keyestudio wiki |
| SR01 / HC-SR04 ultrasonic | **5V** | none (`pulseIn`) or `NewPing` | Trig from GPIO, **Echo through 1k+2k divider** | RNT "ESP32 HC-SR04" |
| PIR Motion | 3V3 | none (`digitalRead`, use an interrupt) | any input GPIO | RNT "ESP32 PIR interrupts" |
| Photoresistor | 3V3 | none (`analogRead`) | ADC1 pin | Keyestudio wiki |
| Sound Sensor | 3V3 | none (`analogRead`/`digitalRead`) | analog on ADC1 | Keyestudio wiki |
| IR Receiver | 3V3 | `IRremote` or `IRremoteESP8266` | any input GPIO | RNT / Keyestudio wiki |
| Flame Sensor | 3V3 | none | digital or ADC1 | Keyestudio wiki |
| Hall Sensor | 3V3 | none | digital or ADC1 | Keyestudio wiki |
| Reed Switch | 3V3 | none (`digitalRead`) | any input GPIO | Keyestudio wiki |
| Tilt Sensor | 3V3 | none | any input GPIO | Keyestudio wiki |
| Collision / Obstacle | 3V3 | none | any input GPIO | Keyestudio wiki |
| Line Tracking | 3V3 | none | digital | Keyestudio wiki |
| Photo Interrupter | 3V3 | none | digital | Keyestudio wiki |
| Analog Gas | **5V** | none (`analogRead`) | **divide the analog out to 3.3V**, warm-up needed | DIYI0T MQ-series |
| Alcohol | **5V** | none (`analogRead`) | **divide the analog out** | as above |
| Steam | **5V** | none (`analogRead`) | **divide the analog out** | Keyestudio wiki |
| Thin-film Pressure | 3V3 | none (`analogRead`) | ADC1 pin | Keyestudio wiki |
| Capacitive Touch | 3V3 | none (digital), or built-in `touchRead` on touch pins | digital out | Keyestudio wiki |
| ADXL345 accelerometer | 3V3 | `Adafruit_ADXL345` + `Adafruit Unified Sensor` | I2C: classic SDA21/SCL22, **S3 remap to 8/9** | Last Minute Engineers "ADXL345" |

### C. Displays
| Display | Volt | Library | Pins / caveat | Code pointer |
|---|---|---|---|---|
| LCD_128x32_DOT | 3V3 | Keyestudio `lcd128_32_io.h` (kit lib) | I2C, S3 remap pins | Keyestudio wiki (use kit lib) |
| HT16K33 8x8 Dot Matrix | 3V3 | `Adafruit LED Backpack` + `Adafruit GFX` (or Keyestudio `HT16K33` lib) | I2C addr 0x70, S3 remap pins | Adafruit LED Backpack guide |
| TM1650 4-Digit Tube | 3V3 | Keyestudio `TM1650.h` (kit lib) | 2-wire, kit lib needed | Keyestudio wiki |

### D. Actuators and outputs
| Actuator | Volt | Library | Pins / caveat | Code pointer |
|---|---|---|---|---|
| 9G Servo | **5V** | `ESP32Servo` | power 5V from expansion/AA, signal from GPIO, common ground | DroneBot "ESP32 Servo" |
| 130 Motor | **5V** | none | **needs a driver or switch via the 5V relay, never direct to a GPIO** | switch via relay |
| Fan | **5V** | none | same rule as the motor | switch via relay |
| Active Buzzer | 3V3 | none (`digitalWrite`) | any safe GPIO | Keyestudio wiki |
| Speaker Module | 3V3 | none (`tone()`/`ledc`) | PWM GPIO. Best sound option in the kit | Keyestudio wiki |
| 3W Loudspeakers | n/a | n/a | **need an amplifier (not supplied)**, use the Speaker Module instead | n/a |
| 5V Relay | **5V** | none (`digitalWrite` to IN) | coil 5V, trigger from GPIO. Switch the motor/fan/laser, **avoid mains** | RNT "ESP32 Relay" |
| Laser Module | 3V3 | none (on/off like an LED) | any safe GPIO. **Do not point at eyes** | Keyestudio wiki |
| 6812 RGB | 3V3* | `Adafruit NeoPixel` (or `FastLED`) | data from GPIO, `NEO_GRB + NEO_KHZ800` | RNT "ESP32 NeoPixel" |

### E. Input devices
| Device | Volt | Library | Pins / caveat | Code pointer |
|---|---|---|---|---|
| Rotary Potentiometer | 3V3 | none (`analogRead`) | ADC1 pin | RNT analog read |
| Rotary Encoder | 3V3 | none (poll CLK/DT/SW) or `AiEsp32RotaryEncoder` | SW uses `INPUT_PULLUP`, avoid strapping pins | SunFounder "Rotary Encoder" |
| Joystick | 3V3 | none (2x `analogRead` + `digitalRead`) | two ADC1 pins + one digital | Keyestudio wiki |
| 5-Channel AD Button | 3V3 | none (`analogRead`, compare thresholds) | one ADC1 pin | Keyestudio wiki |
| Capacitive Touch | 3V3 | none (digital) or built-in `touchRead` | digital out | Keyestudio wiki |

### F. RFID, timekeeping, IR remote
| Project | Volt | Library | Pins / caveat | Code pointer |
|---|---|---|---|---|
| RFID read a card UID | 3V3 | `MFRC522` (SPI) or `MFRC522_I2C` | **RC522 is 3.3V only, never 5V.** Print the UID from the White Card / Key Fob | Keyestudio wiki; MFRC522 examples |
| DS1307 real-time clock | 5V (power) | `RTClib` (Adafruit) or `RtcDS1307` (Makuna) | I2C addr 0x68, power 5V, I2C lines 3.3V fine, S3 remap pins | Adafruit RTClib |
| IR remote decode | 3V3 | `IRremote` / `IRremoteESP8266` | JMFP-4 remote + IR Receiver, decode NEC codes | RNT IR remote |

### G. Connectivity (the Physical AI / IoT payoff)
| Capability | Library | Notes | Code pointer |
|---|---|---|---|
| WiFi basics (STA/AP) | `WiFi.h` (built-in) | both boards | RNT WiFi guides |
| Web server (phone page of readings/buttons) | `WiFi.h` + `ESPAsyncWebServer` + `AsyncTCP` | open the ESP32's IP on a phone on the same WiFi, great live demo | RNT "ESP32 Async Web Server" |
| Bluetooth Classic (32E) | `BluetoothSerial.h` (built-in) | **classic ESP32 only**, the S3 does **not** do BT Classic | Keyestudio wiki |
| BLE (both, the S3's strength) | `BLEDevice.h` (built-in) / Nordic UART | the `claude-desktop-buddy` pattern lives here | anthropics/claude-desktop-buddy |
| MQTT to a dashboard | `PubSubClient` or `Adafruit MQTT` | publish readings to a broker / Adafruit IO | RNT "ESP32 MQTT Publish" |

### Crowd-pleasing combos (point teams here for loop 2)
- **Parking sensor:** SR01 ultrasonic + Active Buzzer (beeps faster as you get closer).
- **Weather station:** XHT11 + LCD_128x32 (or HT16K33).
- **Tripwire / intruder alert:** PIR + 5V Relay + Laser (or Buzzer).
- **Mini game:** Joystick + HT16K33 8x8 dot matrix.
- **Sound-reactive lights:** Sound Sensor + 6812 RGB.
- **Access control:** RFID Reader + 9G Servo + Buzzer (known card opens the "door").
- **Motion-reactive lights:** ADXL345 + 6812 RGB.

### How to use this catalogue on the night
1. Identify the part, jump to its row.
2. Read the team the **library name** (that is literally what they tell Claude Code to install).
3. Give Claude the trio: **board** (`esp32` or `esp32s3`) + **part name** + **library** + the **pins** from the row.
4. For any 5V row, route power through the **expansion board + AA pack**, not the laptop USB.
5. Combine a sensor row with a display or actuator row for the "wow".

---

## 6. Run-of-show, escalation, playbook

### Run-of-show (2.5 hours)
| Time | Block | Facilitator action |
|---|---|---|
| 0:00 to 0:10 | Welcome + the One Rule | Hold up a board: "3.3V logic, not 5V tolerant. 5V parts to the 5V rail, never a GPIO." Point to the Safety one-pager. |
| 0:10 to 0:20 | Setup check | Every team compiles and flashes Blink. Classic: CP2102 driver + "ESP32 Dev Module". S3: native USB + "USB CDC On Boot: Enabled". |
| 0:20 to 0:30 | Pick a seed sensor | Each team grabs ONE part, gets the smallest serial-print demo working. Bank the first win. |
| 0:30 to 1:15 | Build loop 1 | Perplexity > Claude Code > flash > serial. Circulate, run the unsticking checklist. |
| 1:15 to 1:25 | Break / cross-pollinate | Teams show their serial output to a neighbour. |
| 1:25 to 2:05 | Build loop 2, combine for the wow | Push to a sensor + actuator combo (parking sensor, access control, sound-reactive lights). |
| 2:05 to 2:25 | Demos | 90 seconds per team. Working serial counts as a win. |
| 2:25 to 2:30 | Wrap | Point to the kit's own wiki code for anyone continuing at home. |

### Escalation triggers (when to step in)
- **Board resets / `BROWNOUT_RST` / garbage serial:** power problem. Short data cable, direct laptop port, 5V parts on the expansion board + AA pack, shared ground.
- **Upload stalls (`Connecting... _ _ _`):** classic, hold BOOT and release after the percentage appears. S3, BOOT + tap RESET for download mode, confirm the port, enable CDC-on-boot.
- **Sensor reads 0 or 4095 constantly:** it is on an ADC2 pin with WiFi on, or on a flash/strapping pin. Move analog to ADC1 (32 to 39 classic, 1 to 10 S3), avoid 6 to 11 and the strapping pins.
- **`#include` or "does not name a type" errors:** wrong or missing library. Confirm the exact name from the Templates table (often Keyestudio's bundled lib, not the generic one), paste the error into Claude Code.
- **Anything hot or smells of burning:** unplug USB + AA pack immediately, then diagnose miswire, 5V-on-GPIO, or short before re-powering.
- **A team has nothing working at 0:45:** hand them the relevant Keyestudio wiki sketch and an easy 3V3 part, get a serial-print win, then build out.

### Facilitator playbook
1. **Pre-flight before doors open:** install the esp32 boards package + CP2102/CH340 drivers on at least one machine, and personally flash one classic board and one S3 board so you have hit the flashing quirks yourself.
2. **Default everyone to the classic WROOM-32E / Keyestudio Plus + the IO Expansion Board** for the first win. It is the path the Keyestudio code is written for and the keyed connectors prevent polarity mistakes. Reserve the **S3 for BLE / native-USB / "AI agent reacts physically"** demos.
3. **Push serial-print-first, then combine.** The benchmark for "this team is unstuck" is a number moving in the serial monitor. Once every team hits that, switch your coaching to sensor + actuator combos.
4. **Keep 5V discipline visible all night.** The biggest risk to the hardware is a 5V signal on a GPIO or a hungry actuator browning out the USB rail. If you see a servo/motor/relay wired without the expansion-board 5V or AA pack, fix it before power-up.
5. **Thresholds that change your plan:** if more than two teams stall on flashing, do a 3-minute whole-room demo of the BOOT-button / CDC-on-boot fix. If teams finish early, point them at the `claude-desktop-buddy` BLE pattern on an S3 as a stretch goal.

---

## 7. Caveats and assumptions

- **Kit variant.** KS5003 ships with an ESP32 mainboard (KS5004 is the same kit without it). The bundled board is a classic ESP-WROOM-32 (CP2102 USB), treat it identically to the WROOM-32E. The wiki/GitHub code is written and tested against this classic board. On the ESP32-S3 you must apply the S3 safe-pin map and remap I2C, because the S3 has no GPIO22 (the classic SCL default).
- **Pin numbers are examples, not gospel.** Every code pin shown is the Keyestudio default for the classic board. They work, but confirm against the silkscreen of the exact board in front of you, and note Keyestudio sometimes uses strapping pins (for example encoder CLK on GPIO12) that can cause boot or upload hiccups, move to a safe pin if a board misbehaves.
- **The 5V-tolerance "debate" is resolved here in favour of the safe rule.** Some forums claim certain ESP32 pins survive 5V. Do not rely on it. Espressif's datasheets and every reputable guide say 3.3V logic, not 5V tolerant. Power 5V parts from the 5V rail, route only 3.3V-range signals to GPIOs, divide if ever in doubt.
- **Library names.** Several parts use Keyestudio's own bundled libraries (`xht11.h`, `DS18B20.h`, `TM1650.h`, `lcd128_32_io.h`, and the kit's HT16K33 and ADXL345 helpers). Install them from the kit's "Libraries" zip. Where a generic community library also works (Adafruit NeoPixel for the 6812, Adafruit DHT for XHT11, OneWire + DallasTemperature for 18B20, Adafruit_ADXL345, MFRC522 for RFID, IRremote for IR, RTClib for the DS1307, PubSubClient for MQTT, ESP32Servo for the servo), it is noted in the Templates tables.
- **ESP32-S3-WROOM-1 sub-variant matters.** On Octal-PSRAM modules (the common N8R8) GPIO 33 to 37 are consumed internally and unusable. On non-PSRAM variants they are free. If an S3 sketch fails only on those pins, that is why, move to GPIO 4 to 18 or 38 to 42.
- **Bluetooth Classic is classic-only.** The `BluetoothSerial.h` (BT Classic / SPP) path works on the WROOM-32E but not the S3. The S3 does BLE. Route BT Classic demos to the classic boards and BLE demos to either.
- **Soldering boards are off-limits tonight.** The Freenove ESP32-CAM, the 120-piece tactile buttons, the 12 bare 10K pots, and the USB-to-ESP-01 adapters are not part of the plug-and-play flow. Set them aside. The 3W loudspeakers need an amplifier that is not in the kit, so use the kit Speaker Module or Active Buzzer for sound.
- **`claude-desktop-buddy` is real and open-source** (`anthropics/claude-desktop-buddy`, Arduino/ESP32, Nordic UART BLE, reference hardware M5StickC Plus, Developer-Mode only). A full port is more than 2.5 hours. Use it as the pattern, not a tonight deliverable.

---

## 8. Sources

Primary, code-complete references a facilitator can jump straight to:

- **Keyestudio wiki and docs** for the ESP32 42-in-1 Sensor Kit (KS5003/KS5004) and related ESP32 kits, per-lesson Arduino code and bundled libraries: `wiki.keyestudio.com` and `docs.keyestudio.com`, product page at keyestudio.com.
- **Keyestudio GitHub** for kit code repositories (search the keyestudio org for the 42-in-1 / ESP32 kits).
- **Random Nerd Tutorials** (`randomnerdtutorials.com`): ESP32 pinout reference, and tested guides for DHT11, DS18B20, HC-SR04, PIR (interrupts), relay, servo, NeoPixel, async web server, and MQTT.
- **Last Minute Engineers** (`lastminuteengineers.com`): ESP32 ADXL345 and sensor guides.
- **DroneBot Workshop** (`dronebotworkshop.com`): "Using Servo Motors with the ESP32".
- **Adafruit Learn**: NeoPixel, LED Backpack (HT16K33), ADXL345, RTClib, and the **`anthropics/claude-desktop-buddy`** ESP32-over-BLE reference project (Adafruit blog, April 2026).
- **Espressif** datasheets and ESP-IDF docs for the ESP32 and ESP32-S3 (GPIO matrices, strapping pins, ADC1/ADC2 with WiFi, native USB / USB-Serial-JTAG, USB CDC On Boot).
- **PlatformIO** docs (`docs.platformio.org`) and **arduino-cli** usage for the build/flash commands and board IDs.
- **SunFounder** docs for the rotary encoder on ESP32.

*This pack is complete and self-contained: Inventory, Safety, Workflow, Bottlenecks, Templates, Run-of-Show, and Caveats, with sources cited inline so you can jump to working code if needed. Hand it out as-is.*
