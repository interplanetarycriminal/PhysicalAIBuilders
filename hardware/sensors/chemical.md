# Chemical / gas / scent sensors

Specific gases (CO, CO₂, CH₄, H₂, O₂, O₃, NO₂, VOCs), odor arrays (e-noses). The "what's in the air?" stack (specific-species edition — see [`environmental.md`](environmental.md) for the broad IAQ stack).

## Part catalog

| Part | Species | Interface | Note |
|---|---|---|---|
| **MQ-2** | LPG, propane, methane, smoke | Analog | Cheap MOS; broad selectivity. Good for "is there smoke?". |
| **MQ-3** | Ethanol / alcohol | Analog | DIY breathalyzer. |
| **MQ-4** | Methane / natural gas | Analog | Leak detector. |
| **MQ-7** | Carbon monoxide | Analog (needs heater cycling) | Cheap CO; cycling demands careful ADC timing. |
| **MQ-135** | Air quality (NH3, NOx, alcohol) | Analog | Very broad; use with skepticism. |
| **CCS811** | eCO₂, TVOC | I²C | Deprecated but still common; burn-in required. |
| **ENS160** | eCO₂, TVOC | I²C | Replaces CCS811; better thermal. |
| **ZP07-MP503** | VOC | Analog | Industrial-grade VOC. |
| **SGP40 / SGP41** | VOC / NOx index | I²C | Sensirion; index is stable output. |
| **DFRobot SEN0466 (MEMS CO)** | CO | I²C / UART | Compact CO, calibrated. |
| **Alphasense electrochemical** | NO₂, O₃, SO₂, H₂S, etc. | Analog via ISB | Professional air-quality class. |
| **Figaro TGS series** | Individual gases | Analog | The OG Japanese MOS sensors. |
| **Winsen ZE03 / ZE07** | Single-species (e.g. O₂, H₂S) | UART | Plug-and-play; calibrated modules. |
| **Bosch BME688** | 8-class ML gas | I²C | Train it on your own scents (coffee vs smoke vs burnt toast). |

## Interface notes

- MOS (metal-oxide) sensors need a heater — budget for current (~150 mA for MQ series) and warmup (~60 s).
- Electrochemical cells are the gold standard for specific species but need bias boards (e.g. Alphasense ISB).
- MOS sensors age and drift — always give users a "calibrate now in fresh air" affordance.

## Capability contribution

- **Find**: leak / spike / intrusion (a gas spike at 03:00 means something specific).
- **Transform**: raw resistance → ppm via calibration curve.
- **Distill**: daily / weekly exposure totals.
- **Reason**: fuse with BME680 + mic ("this is cooking, not a fire").
- **Do**: close exhaust fan, open window servo.

## Pairs with (asymmetry combos)

- **BME688 (trained) + microphone** — classify cooking method *and* detect "pan about to burn" by sizzle-pitch change.
- **O₂ sensor + CO₂ sensor + RH** — cellar / wine-room / fermentation safety.
- **CH₄ + GPS + LoRa** — leak-detection walk around a gas field (citizen-science, methane map).
- **BME688 + e-paper** — pocket "scent diary" that logs what your house smelled like during dinner last Tuesday.

## Where to buy

- **SparkFun / Adafruit** for SGP4x, ENS160, SCDx breakouts.
- **DFRobot / Seeed** for broad Grove-compatible gas sensors.
- **Alphasense** direct for professional electrochemical.
- **Figaro** via distributors for TGS series.
- **DigiKey / Mouser** for Sensirion, Bosch.

## Gotchas

- **Cross-sensitivity** — the "CO" sensor you bought also reads ethanol. Always look up the cross-sensitivity table.
- **Calibration drift** — MOS sensors drift 10–20%/year; recalibrate by exposure to clean outdoor air.
- **Interference with airflow** — enclosures that block airflow also block the sensor; vent the housing.
- **Medical claims** are again regulated; exposure dashboards for personal curiosity are fine, clinical outputs are not.
