# Parent Sleep Ally

> **Level:** 40 — Multi-sensor Distill. One week. ~$80.
> **Tribe:** Domestic heroes (#71)
> **One-line vibe:** a bedside object that *just knows* the night was okay, and tells you one honest sentence in the morning.

## The asymmetry

Baby monitors are mostly fear-amplifiers — raw audio, raw video, noisy alerts. Sleep Ally does the opposite: it *distills* the night into one honest morning sentence, because its sensors (radar breathing, air quality, ambient audio events) let it know what most parents can only guess at.

## The two-line spec

```
HARDWARE:  ESP32 + LD2410B (mmWave) + SCD41 (CO₂/T/RH) + INMP441 (I²S mic) + 2.9" e-paper
SOFTWARE:  on-device sensor fusion + MQTT to home broker + an LLM that writes the AM line
```

## Bill of materials

| Qty | Part | Role | Vendor | Approx $ |
|---|---|---|---|---|
| 1 | ESP32 DevKitC (classic, for CSI-ready future) | MCU | Adafruit | $7 |
| 1 | HLK-LD2410B mmWave | Breathing / presence | AliExpress | $5 |
| 1 | SCD41 breakout | CO₂ + T + RH | Sensirion / SparkFun | $45 |
| 1 | INMP441 I²S mic | Ambient audio events | Adafruit / AliExpress | $6 |
| 1 | 2.9" Waveshare e-paper | Morning sentence | Waveshare | $20 |
| 1 | USB-C PSU | Power | anywhere | $5 |
| 1 | Wooden / ceramic enclosure | Calming form | any | $10 |

**Total:** ~$98.

## Wiring

```
LD2410B  TX ──► ESP32 GPIO 16 (RX2)
         RX ──► ESP32 GPIO 17 (TX2)
         VCC ──► 5V, GND ──► GND

SCD41    SDA ──► GPIO 21  SCL ──► GPIO 22

INMP441  WS  ──► GPIO 25  SCK ──► GPIO 26  SD ──► GPIO 33

e-paper  BUSY── GPIO 4  RST── GPIO 5  DC── GPIO 17  CS── GPIO 15
         CLK── GPIO 18  DIN── GPIO 23
```

Single 5 V rail; separate 3.3 V regulator for the mic (cleaner supply).

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). PlatformIO project, Arduino core, libs: `LD2410`, `SparkFun_SCD4x`, `ESP_I2S`, `GxEPD2`, `PubSubClient`.

Design choices:

- Two FreeRTOS tasks: a *sensing* task that polls LD2410 (5 Hz), SCD41 (0.1 Hz), and computes mic RMS in 2 s windows; a *representation* task that only redraws the e-paper at 06:30 or on explicit parental tap.
- Audio events (cry-like peaks) are *counted*, not recorded — privacy by hardware.
- Night summary JSON is published at 06:00 to `sleep-ally/<room>/night-summary`.

## Companion software

See [`companion/README.md`](companion/README.md). A tiny Home Assistant-compatible service that receives the JSON and asks Claude for the morning line (with prompt caching for the parent-voice template).

## Ethics of this asymmetry

- **No recording.** Mic computes RMS and event counts only — raw samples never leave RAM.
- **Visible indicator.** The LED on the mic breakout is exposed through the enclosure.
- **Local by default.** MQTT broker is on the home LAN; Claude use is optional (offline template fallback).
- **No cameras.** Deliberate design choice; see [`../../capabilities/find.md`](../../capabilities/find.md) on privacy-first presence sensing.

## Extension paths (climbing the ladder)

- **+10 (level 50):** train a TinyML classifier (Edge Impulse) to distinguish cry vs cough vs non-event on-device. Still no recording; just better labels.
- **+20 (level 60):** mesh with a second unit by the parent bed; haptic pager at that unit when the baby unit sees a concerning pattern.
- **+30 (level 70):** enclosure revision, battery-first, OTA updates — a shippable object.

## Credits & inspiration

The sleep-research community's *breathing-rate-via-radar* papers, the Sensirion IAQ index design, and the honest idea that a parent needs **less** data, not more — just the right one sentence.
