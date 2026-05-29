# Kit Weather Station

> **Level:** 20 — Connected Object. One session. Kit-only.
> **Tribe:** Domestic heroes
> **One-line vibe:** a little readout that always knows the room's temperature and humidity.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. A
> [build-night](../../build-night/) crowd-pleaser. All parts are 3.3V-safe; on the **S3** remember
> there is no GPIO22 — remap I²C. See
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## The asymmetry

A thermometer tells you *now*; a connected readout that you can also reach from your phone tells you
*the room you're not in*. The first time the data leaves the bench and lands on a phone, the build
crosses from "toy" to "instrument" — the wielder knows conditions in a space they can't see.

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + XHT11 (DHT11-compatible) + LCD_128x32 (I²C)
SOFTWARE:  on-device read + LCD render; serial print; optional Wi-Fi phone page
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | XHT11 Temp/Humidity (DHT11-compatible) | Sense T + RH | Keyestudio 42-in-1 kit | in kit |
| 1 | LCD_128x32_DOT module (I²C) | Readout | Keyestudio 42-in-1 kit | in kit |

**Total:** ~$0 beyond the kit.

## Wiring

```
XHT11 / DHT11               ESP32 (classic)
 DATA ──► GPIO4
 V    ──► 3V3
 G    ──► GND

LCD_128x32 (I²C)
 SDA ──► GPIO21             (S3: remap, e.g. Wire.begin(8, 9))
 SCL ──► GPIO22             (⚠ GPIO22 does NOT exist on the S3)
 V   ──► 3V3   G ──► GND
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework. Libraries: Adafruit `DHT sensor
library` + `Adafruit Unified Sensor` (treat XHT11 as a DHT11), and the Keyestudio kit display lib
`lcd128_32_io.h` (install from the kit "Libraries" zip).

Key choices:

- The DHT11 is slow (~0.5 Hz) and occasionally returns `NaN`; the loop **retries** rather than
  printing garbage.
- Serial print is the guaranteed win; the **LCD render** is a clearly-marked step to finish with
  Claude Code, since the kit display lib's API differs from generic ones.

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first, then a phone web page as the +10 —
the moment the readings leave the desk.

## Ethics of this asymmetry

- Temperature and humidity are **non-personal** environmental data. Low privacy surface.
- If you later log a *shared* space's data to the cloud, keep retention short and disclose it — see
  [`../../framework/information-asymmetry.md`](../../framework/information-asymmetry.md) §"Ethics of Asymmetry".

## Extension paths (climbing the ladder)

- **+10 (level 30):** swap the LCD for the **HT16K33 8×8** or the **TM1650** 4-digit tube.
- **+10 (level 20):** serve a phone web page of live T/RH (templates §G).
- **+20 (level 40):** add the **NTC thermistor** as a second source and show the disagreement —
  the first taste of sensor fusion (see [`../../capabilities/distill.md`](../../capabilities/distill.md)).

## Credits & inspiration

Random Nerd Tutorials "ESP32 DHT11"; the Keyestudio XHT11 + LCD128x32 lessons. The bedside weather
clock, demystified.
