# Beekeeper HiveLink

> **Level:** 70 — Field Deployment. 4–6 weeks. ~$300.
> **Tribe:** Nature & survival (#24)
> **One-line vibe:** a solar-powered hive sentinel that predicts swarms *days* ahead by listening to the bees.

## The asymmetry

Good beekeepers inspect once a week; the bees can prep to swarm in days. HiveLink watches 24/7 — weight trend, hive temperature profile, VOC / PM in entrance air, and (crucially) the *acoustic tonal shift* that precedes swarming. You get a swarm-date forecast on your phone instead of losing a colony.

## The two-line spec

```
HARDWARE:  ESP32-S3 + HX711 + 4× load cell (full-hive scale) + piezo contact mic inside hive + SEN55 +
           4× DS18B20 in brood area + solar panel + 6 Ah LiPo + LoRa SX1262
SOFTWARE:  PlatformIO firmware with FOTA + LoRa→gateway Pi→web dashboard + LLM forecast notebook
```

## Bill of materials

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 1 | Unexpected Maker TinyS3 (or FeatherS3) | MCU | Unexpected Maker | $20 |
| 1 | HX711 ADC | Load-cell reader | SparkFun | $10 |
| 4 | 50 kg load cells (wheatstone-assembled) | Full-hive scale | AliExpress | $25 |
| 1 | Piezo contact mic + preamp | Acoustic pickup | Cold Gold / DIY | $15 |
| 1 | Sensirion SEN55 | PM + VOC + NOx + T/RH | Sensirion / DigiKey | $55 |
| 4 | DS18B20 waterproof probes | Brood-area temp | AliExpress | $3 each |
| 1 | Heltec WiFi LoRa 32 V3 (optional in place of TinyS3) | MCU + LoRa | Heltec | $25 |
| 1 | SX1262 LoRa breakout (if using TinyS3) | Long-range radio | EBYTE / AliExpress | $8 |
| 1 | 10 W solar panel + Voltaic kit | Energy harvest | Voltaic | $45 |
| 1 | 6 Ah LiPo + BMS | Storage | 18650BatteryStore | $25 |
| 1 | IP65 enclosure + cable glands | Field housing | Hammond / Bopla | $30 |
| 1 | Raspberry Pi 4 (gateway) | LoRa gateway | adafruit | $65 |

**Total:** ~$330.

## Wiring

```
[TinyS3]──I²C── SEN55
       ──I²C── BME280 (optional, outside hive)
       ──1-Wire── DS18B20 ×4 (daisy chain on one data line, 4.7kΩ pull-up)
       ──SPI── SX1262
       ──GPIO── HX711 DT, SCK
       ──I²S── piezo mic via preamp → I²S ADC path (or analog through ESP32 ADC)
Solar → MPPT charger → 6 Ah LiPo → 3V3/5V buck → TinyS3
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). PlatformIO, ESP-IDF or Arduino-ESP32. Uses FreeRTOS:

- **Sense task** — 1 Hz weight, 0.1 Hz SEN55, 1 Hz temperatures, 2 s mic FFT bands.
- **Distill task** — rolling 1 min, 15 min, 24 h summaries.
- **Radio task** — transmits summary every 15 min via LoRa.
- **OTA hook** — on fresh sunrise battery state (>13 V), listens for FOTA updates from gateway.

Swarm-prediction feature seeds:

- Rising weight + sudden drop (≥ 1 kg in 10 min) = likely swarm event (reactive).
- Tonal shift in piezo FFT (larger energy in 200–400 Hz band relative to 100–200 Hz band, rising over 3 days) = *pre-swarm warning* (predictive).
- Brood-area temp instability (SD > 0.4 °C at stable ambient) = queen issue.

## Companion software

See [`companion/README.md`](companion/README.md). A Pi gateway runs [ChirpStack](https://chirpstack.io) or a simple custom LoRa listener, ingests summaries into SQLite / InfluxDB, a web dashboard renders the last 30 days, and a Claude notebook writes the weekly forecast + recommended intervention.

## Ethics of this asymmetry

- Bees are not humans, but bees are communities; be careful not to disturb the hive with sensor cabling (wood screws, not glue).
- Data-share with neighboring beekeepers as opt-in community — asymmetry of a single keeper scales into a regional forecast.

## Extension paths (climbing the ladder)

- **+10 (level 80):** Claude-driven agent checks BOM, weather, and your calendar to schedule inspections.
- **+20 (level 90):** add a Grove Vision AI V2 at the entrance to count foragers and classify pollen color.
- **+30 (level 100):** publish a multi-apiary longitudinal dataset; partner with a university bee lab.

## Deployment checklist

1. Calibrate load cells indoors with known weights (5, 10, 20 kg).
2. Weatherproof every connector; silicone the SEN55 intake.
3. Power-budget with an INA226 for one full day indoors before deploying.
4. Place the LoRa antenna above hive roof, vertical polarization, unobstructed south.
5. Initial FOTA test from the gateway before you're standing in the field.

## Credits & inspiration

The [HiveMind](https://hivemind.co) and [Arnia](https://arnia.co) commercial products — we're building the open-source community equivalent. Acoustic bee research at Nottingham Trent, Purdue.
