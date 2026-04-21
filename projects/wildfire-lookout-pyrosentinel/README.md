# Wildfire Lookout — PyroSentinel

> **Level:** 90 — Co-processor-class. Months. ~$500+ per node.
> **Tribe:** High-stakes operations (#91)
> **One-line vibe:** a ridge-top device that watches for smoke, heat, and wind-shift — mesh-networked across a valley, and feeds a SitRep-writing agent.

## The asymmetry

State fire lookouts sometimes can't see the valley for the haze; citizen cameras catch smoke after it's already bad. PyroSentinel nodes, placed on ridges with clear line-of-sight to a basin, combine thermal imaging, particulate sensing, wind direction, and humidity into a single mesh that surfaces *early* smoke plumes with quantified confidence — hours before the first call would come in.

## The two-line spec

```
HARDWARE:  ESP32-S3 + MLX90640 thermal + SEN55 PM + BME280 + AS3935 lightning
           + anemometer + wind-vane (AS5600) + LoRa SX1262 + 20 W solar + 20 Ah LiFePO4
SOFTWARE:  ESP-IDF firmware + gateway at fire ops + ops dashboard + LLM SitRep agent
```

## Bill of materials (per node)

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 1 | ESP32-S3-DevKitC-1 N16R8 | MCU | Espressif / DigiKey | $15 |
| 1 | MLX90640 32×24 thermal | Thermal plume | SparkFun / Adafruit | $70 |
| 1 | Sensirion SEN55 | PM + VOC + NOx | DigiKey | $55 |
| 1 | BME280 | T/RH/P | Adafruit | $10 |
| 1 | AS3935 + coil | Lightning | Sparkfun | $25 |
| 1 | Anemometer (pulse/rev hall) | Wind speed | DIY / DFRobot | $15 |
| 1 | AS5600 + magnet on vane | Wind direction | AliExpress | $5 |
| 1 | SX1262 LoRa breakout (or RAK4200) | Long-range mesh | EBYTE / RAK | $20 |
| 1 | 20 W solar panel + Voltaic MPPT | Solar harvest | Voltaic | $90 |
| 1 | 20 Ah LiFePO4 + BMS | Storage (cold-weather-safe) | BigBattery | $120 |
| 1 | IP66 polycarbonate enclosure + RF-transparent top | Housing | Hammond | $60 |
| 1 | Grounded mast + pole-mount | Install | hardware store | $30 |

**Total:** ~$500 per node. Deploy 5–10 for a valley mesh.

## Wiring / mechanical

- MLX90640 needs RF-transparent (IR-transparent) window or open aperture — no normal glass.
- SEN55 intake faces down, protected by a mesh to keep insects out.
- Anemometer and vane mounted on a small boom above the enclosure.
- LoRa antenna vertical polarization, above every other metal structure on the node.
- Grounding for the AS3935 coil and enclosure mandatory — these are on ridges; lightning is the *subject* but also the *threat*.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). ESP-IDF, dual-core:

- Core 0: LoRa, sensor sampling, FreeRTOS scheduling, FOTA.
- Core 1: MLX90640 frame processing — computes plume features (hotspot size, centroid, temperature distribution) every 2 s.

Smoke-plume heuristic (tunable):
- PM2.5 > 50 µg/m³ for > 5 min  AND
- thermal frame shows any pixel > ambient + 15 °C for > 10 s  AND
- wind direction stable → **confidence = high**; encode centroid bearing from node.
- Lightning within 10 km in the last 60 min raises a separate `ignition-possible` flag.

## Companion software

See [`companion/README.md`](companion/README.md). Three parts:

1. **Gateway** — a small laptop / NUC at fire ops running a LoRa listener + ChirpStack; writes every packet to Timescale/Postgres.
2. **Ops dashboard** — Grafana + a Mapbox overlay showing each node's status, wind arrows, PM heatmap, plume centroids as shaded sectors.
3. **SitRep agent** — every 30 min, Claude is handed the rollups + the current deployment metadata and asked to write a SitRep in the ICS-201 style. Humans review + forward.

## Ethics of this asymmetry

- These are **operations** devices; deploy only in coordination with local fire authorities or the land manager.
- Do not publicly publish node locations — plume centroid coordinates can be misused.
- Include a physical "mute" button on the node that suppresses LoRa output for 1 hour (for maintenance without noise on the ops channel).

## Extension paths (climbing the ladder)

- **+10 (level 100):** partner with a fire-research group; publish a season-long labeled dataset.
- Cross-build with [`../cold-case-sleuth-timefold/`](../cold-case-sleuth-timefold/) — the same temporal correlator engine answers "when/where did the first smoke appear?"

## Deployment checklist

1. Drop-test enclosure cold (0 °C) + hot (60 °C) with full electronics.
2. Field-commission by walking a smoke source past the node — verify detection + bearing.
3. Two-week lab burn-in under synthetic-sun lamp before field deploy.
4. Maintenance calendar: SEN55 replacement in 2 years; battery inspection quarterly.

## Credits & inspiration

Open Climate Fix, AlertWildfire, the BurnPro pro-field instruments whose prices we don't have to match, and the volunteer fire-watch communities that make mesh-of-ridges possible.
