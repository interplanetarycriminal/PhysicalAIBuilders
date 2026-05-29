# Kit Parking Sensor

> **Level:** 10 — First Sense. One session. Kit-only.
> **Tribe:** Domestic heroes
> **One-line vibe:** a buzzer that beeps faster the closer you get — your ears suddenly know distance.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. This is one
> of the [build-night](../../build-night/) crowd-pleasers. Read
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md) first — the
> ultrasonic is a **5V** part and its Echo line must be divided down before a GPIO.

## The asymmetry

Your eyes are bad at the last 30 cm — the exact range where you dent a bumper or clip a shelf. An
ultrasonic ping measures it to the centimetre and the buzzer turns that into a tempo your body reads
instinctively: slow beeps = space, frantic beeps = stop. The wielder *hears* distance, a sense
nobody in the room otherwise has.

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + SR01/HC-SR04 ultrasonic (5V) + Active Buzzer
SOFTWARE:  on-device distance→tempo mapping; serial print of cm; optional phone web page
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | SR01 / HC-SR04 ultrasonic | Distance (5V) | Keyestudio 42-in-1 kit | in kit |
| 1 | Active Buzzer module | Tempo output | Keyestudio 42-in-1 kit | in kit |
| 1 | ESP32-IO Expansion Board + 6×AA holder | Clean 5V rail for the sensor | Keyestudio 42-in-1 kit | in kit |
| 2 | Resistors 1 kΩ + 2 kΩ | Echo divider to 3.3V | (or read Echo through the expansion header) | in kit / pack |

**Total:** ~$0 beyond the kit.

## Wiring

```
SR01 ultrasonic            ESP32 (classic)
 VCC ──► 5V (expansion/AA rail, NOT the 3.3V pin)
 GND ──► GND (common ground with the ESP32)
 TRIG ─► GPIO17                      (3.3V out from ESP32 is fine)
 ECHO ─► 1kΩ ──┬──► GPIO16           (sensor drives ECHO at 5V — divide it!)
               │
             2kΩ
               │
              GND

Active Buzzer
 SIG ──► GPIO4
 V   ──► 3V3
 G   ──► GND
```

See the divider diagram in [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md#the-1k--2k-divider-for-the-ultrasonic-echo-and-5v-analog-outs).

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework; no extra library needed (`pulseIn`),
or install `NewPing` for cleaner timing.

Key choices:

- `TRIG` is driven by the ESP32 at 3.3V (fine); only the **5V-driven `ECHO`** goes through the
  1k+2k divider.
- Distance is mapped to an **inter-beep gap** (closer → shorter gap), so the tempo encodes range.
- A `pulseIn` timeout treats "no echo" as "far" so the loop never stalls.

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first: watch centimetres in the monitor at
115200. The +10 extension serves a tiny web page so a phone on the same Wi-Fi sees the live distance.

## Ethics of this asymmetry

- Ultrasonic ranging is **non-identifying** — it measures distance, not people. No privacy surface.
- The buzzer is audible; nothing here records or transmits. A truly local sense.

## Extension paths (climbing the ladder)

- **+10 (level 20):** add Wi-Fi + an async web page showing live distance — see
  [`../../build-night/templates-catalogue.md`](../../build-night/templates-catalogue.md) §G.
- **+10 (level 20):** swap the buzzer tempo for the **6812 RGB** going green→amber→red.
- **+20 (level 30):** log distance over time and detect "a car arrived" vs "a person walked past".

## Credits & inspiration

Random Nerd Tutorials "ESP32 HC-SR04"; the Keyestudio kit ultrasonic lesson. The car-reversing
sensor every driver already trusts, rebuilt in an hour.
