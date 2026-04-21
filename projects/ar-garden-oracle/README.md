# AR Garden Oracle

> **Level:** 30 — Opinionated Firmware. A weekend. ~$60.
> **Tribe:** Domestic heroes (#74)
> **One-line vibe:** point your phone at a plant; the plant tells you what it needs, sourced from its own sensors.

## The asymmetry

Most gardeners look at a plant and guess. AR Garden Oracle reads the plant's soil moisture, spectral reflectance (via a small probe), and ambient light, then narrates — over AR — exactly which plant in your bed needs which intervention today.

## The two-line spec

```
HARDWARE:  ESP32-S3 + AS7341 (11-channel) + capacitive soil moisture array + TSL2591 + BLE tag per pot
SOFTWARE:  PlatformIO firmware + phone web-AR overlay + Claude oracle that narrates the snapshot
```

## Bill of materials

| Qty | Part | Role | Vendor | Approx $ |
|---|---|---|---|---|
| 1 | Unexpected Maker FeatherS3 | MCU | Unexpected Maker | $25 |
| 1 | AS7341 breakout (Qwiic) | Spectral sensor | SparkFun | $17 |
| 1 | TSL2591 breakout | Ambient light | Adafruit | $8 |
| 3–6 | Capacitive soil moisture sensor v1.2 | Per-pot probe | AliExpress | $2 each |
| 1 | 3× AAA holder / 1S LiPo + charger | Power | Adafruit | $6 |
| 6 | BLE beacon tag (iBeacon or cheap nRF) | Pot identifiers | AliExpress | $3 each |
| 1 | 3D-printed wand enclosure | Form factor | print local | — |

**Total:** ~$70 for 4 pots.

## Wiring

```
FeatherS3 I²C (SDA/SCL/3V3/GND) ─► AS7341 & TSL2591 (Qwiic chain)
FeatherS3 ADC pins ─► soil-moisture probe AOUT  (one per pot, multiplexed)
FeatherS3 BLE radio ─── scans for pot beacons
```

Each pot has a BLE beacon whose MAC maps (via a config file) to a plant species.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Built with PlatformIO + Arduino-ESP32. Uses a FreeRTOS task for the AS7341 integration (~500 ms per scan) so the UI stays responsive.

Key choices:

- The wand uses a momentary button: press → take a reading *right there* and exposes it as a BLE GATT characteristic.
- The phone discovers both the wand and the pot beacon; infers which plant was scanned via RSSI.
- Reading payload is JSON: `{"ts":…, "lux":…, "spectrum":[…], "soil":…, "pot_mac":"…"}`.

## Companion software

See [`companion/README.md`](companion/README.md). A Progressive Web App using WebBLE + Claude. The AR overlay is simple: image of the plant from your camera, with a labeled bubble floating above it.

## Ethics of this asymmetry

Non-human subjects — ethical surface is small. Share readings in a community corpus if you want to help build a species-vs-spectrum training set.

## Extension paths (climbing the ladder)

- **+10 (level 40):** add a small water-pump ring (level 40 actuation) — the oracle can *water* on its own when soil < threshold.
- **+20 (level 50):** train an Edge Impulse classifier on species → healthy spectrum, alert on deviation on-device.
- **+30 (level 60):** mesh wands (ESP-NOW) across a whole community garden.

## Credits & inspiration

PlantNet, iNaturalist, the [Koala Collective](https://) of hobbyist spectral gardeners — and the AS7341 community writeups from SparkFun.
