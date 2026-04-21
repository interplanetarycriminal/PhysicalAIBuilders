# FermentDoc — homebrewer's batch archivist

> **Level:** 20 — Connected Object. One evening. ~$40.
> **Tribe:** Domestic heroes (#75)
> **One-line vibe:** every batch of cider, beer, or kombucha you've ever brewed, measured like a lab — without the lab.

## The asymmetry

A brewer without FermentDoc guesses at primary-ferment temperature, gravity drop, and fermentation pause. A brewer **with** FermentDoc has a minute-by-minute graph of everything, a shareable batch record, and an LLM that explains *why* this batch tastes different from the last.

## The two-line spec

```
HARDWARE:  ESP32 DevKit + DS18B20 (waterproof) + BME280 + Tilt-compatible BLE hydrometer (optional)
SOFTWARE:  Wi-Fi → MQTT → a tiny Flask/Grafana dashboard + an LLM batch-notebook
```

## Bill of materials

| Qty | Part | Role | Vendors | Approx $ |
|---|---|---|---|---|
| 1 | ESP32 DevKitC | MCU + Wi-Fi | Adafruit, AliExpress | $7 |
| 1 | DS18B20 waterproof probe | Wort temp | AliExpress, Adafruit | $4 |
| 1 | 4.7 kΩ pull-up resistor | for DS18B20 data line | everywhere | $0 |
| 1 | BME280 breakout (I²C) | Ambient T/RH/P | Adafruit, SparkFun | $5 |
| 1 | Tilt Hydrometer (optional) | Gravity via BLE | Tilt Hydrometer site | $70 |
| 1 | Screw-cap + 3D-printed gland | Sealed probe entry | print local | — |
| 1 | USB-C PSU | Power | anywhere | $5 |

**Total:** ~$21 without Tilt, ~$91 with.

## Wiring

```
DS18B20 (yellow data) ─┬─ GPIO 4
                       └─ 4.7k to 3V3
DS18B20 (red) ─ 3V3
DS18B20 (black) ─ GND

BME280 SDA ─ GPIO 21
BME280 SCL ─ GPIO 22
BME280 VCC ─ 3V3, GND ─ GND
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino IDE, libs: `OneWire`, `DallasTemperature`, `Adafruit_BME280`, `PubSubClient`.

Key design choices:

- **MQTT-first.** Posts to topics `fermentdoc/<batch>/{temp_wort,temp_amb,rh,p,gravity}` every 30 s.
- **Batch ID** written to NVS; press BOOT button to start a new batch.
- **LED status**: breathing green = fermenting (CO₂-inferred gravity drop > 0.002 per day); red = fermentation stalled; blue = out-of-range T.

## Companion software

See [`companion/README.md`](companion/README.md). Two pieces:

1. **Local dashboard** — a 50-line Flask app subscribing to the MQTT broker; Chart.js live graph; per-batch persistent store (SQLite).
2. **LLM batch notebook** — at end-of-batch, Claude is given the time-series JSON + the recipe; writes a tasting-note prediction and a next-batch tweak.

## Ethics of this asymmetry

Negligible — your own beer fridge. But: keep the LLM companion on-device if you're a commercial producer; your recipes are trade secrets.

## Extension paths (climbing the ladder)

- **+10 (level 30):** move the dashboard to PlatformIO + ESP-IDF; add OTA from Git.
- **+20 (level 40):** add a CO₂ venturi (gas-bubble counter) and a pH probe — FermentDoc becomes a biology-grade log.
- **+30 (level 50):** train a TinyML classifier to label the fermentation stage (lag, primary, diacetyl-rest, finish) from the temp/ gravity curve.

## Credits & inspiration

[Tilt Hydrometer](https://tilthydrometer.com) for the gravity piece. [BrewPi](https://brewpi.com) for the fermentation-control heritage. FermentDoc's twist: LLM-driven batch notebooks.
