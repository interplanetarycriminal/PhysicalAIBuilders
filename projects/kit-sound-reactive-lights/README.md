# Kit Sound-Reactive Lights

> **Level:** 10 — First Sense. One session. Kit-only.
> **Tribe:** Arts & performance
> **One-line vibe:** the room's noise becomes light — claps, beats and voices paint the strip.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. A
> [build-night](../../build-night/) crowd-pleaser. All parts are 3.3V-safe — see
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## The asymmetry

Sound is invisible and fleeting; most people only feel a room's energy in hindsight. Sample the
sound sensor fast enough and you can *render* that energy in real time on the 6812 RGB — the wielder
sees the beat land a fraction before the crowd consciously registers it. A VU meter for the vibe of
the room.

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + Sound Sensor (analog) + 6812 RGB module
SOFTWARE:  on-device baseline + level→pixels mapping; serial print of the level
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | Sound Sensor module | Noise level (analog) | Keyestudio 42-in-1 kit | in kit |
| 1 | 6812 RGB module (WS2812-type) | Light output | Keyestudio 42-in-1 kit | in kit |

**Total:** ~$0 beyond the kit.

## Wiring

```
Sound Sensor                ESP32 (classic)
 AO (analog) ──► GPIO34      (ADC1, input-only pin — perfect for analogRead)
 V          ──► 3V3
 G          ──► GND

6812 RGB
 DIN ──► GPIO4               (data; a 330 Ω series resistor is good practice)
 V   ──► 5V for best colour (3V3 works for a handful of pixels)
 G   ──► GND (common with the ESP32)
```

> Use **ADC1 (GPIO 32–39)** for the sound input — ADC2 pins stop reading once Wi-Fi is on. GPIO34 is
> input-only, which is exactly what an analog sensor wants. See
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework with `Adafruit NeoPixel`.

Key choices:

- A short **baseline** sample at boot learns the ambient quiet level so the meter isn't biased by the
  sensor's DC offset.
- We light a number of pixels proportional to the **deviation** from baseline, and sweep the hue over
  time so loud moments are vivid, not just bright.

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first: watch the level number jump when you
clap. The +10 extension exposes a phone slider to set sensitivity live.

## Ethics of this asymmetry

- The sound sensor reads **level only**, not audio — nothing is recorded or recognisable as speech.
- Lights are local and visible; there is no covert capture here. (If you ever add a microphone that
  *records*, re-read [`../../framework/information-asymmetry.md`](../../framework/information-asymmetry.md)
  §"Ethics of Asymmetry".)

## Extension paths (climbing the ladder)

- **+10 (level 20):** add a phone web page with a sensitivity slider (writes a threshold over Wi-Fi).
- **+10 (level 20):** beat-detect (rolling peak) and flash on transients instead of raw level.
- **+30 (level 40):** FFT the signal (see [`../../capabilities/transform.md`](../../capabilities/transform.md))
  to map bass→red, mids→green, treble→blue.

## Credits & inspiration

Random Nerd Tutorials "ESP32 NeoPixel"; the Keyestudio sound sensor lesson. Every nightclub VU wall,
shrunk to a desk module.
