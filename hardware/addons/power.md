# Power

Batteries, charging, solar, PMICs, buck/boost, protected cells, deep-sleep design.

## The power question to answer first

> "How long does this build live between plugs?"

Your answer picks the battery chemistry, capacity, and how aggressively you need to sleep.

| Target lifetime | Typical solution |
|---|---|
| Always plugged in | USB-C + maybe a supercap for graceful shutdown. |
| Hours (wearable) | 200–500 mAh LiPo. |
| A week of field logging | 2500–6000 mAh 18650 cell + careful deep-sleep. |
| A season | 18650 pack + small solar + PMIC. |
| A year+ | Primary lithium (LTC-30Ah-class) + ULP wake patterns + LoRa. |

## Part catalog

| Part | Role | Note |
|---|---|---|
| **TP4056** | Single-cell LiPo charger | Everywhere; add a protection IC (DW01A + FETs) separately. |
| **MCP73831** | Single-cell LiPo charger | Better than TP4056; used on Adafruit Feathers. |
| **BQ25895** | USB-C-PD charger + boost | Handles 2-cell + path-management; modern boards. |
| **BQ25628** | Tiny Li charger | Used on Unexpected Maker S3 boards. |
| **MCP73871** | Solar-aware Li charger | Prefer the solar input; "power path" charger. |
| **MT3608** / **TPS61023** | Boost converter | Cheap 5 V from 3.7 V. |
| **AMS1117-3.3** | Linear 3.3 V | Cheap + common but wasteful; avoid for battery builds. |
| **TPS62840 / MCP1810** | Low-Iq buck / LDO | Quiescent current < 1 µA — essential for long-life deep-sleep. |
| **TPL5110** | Nanopower timer | Wakes the ESP32 at a set interval; ~35 nA quiescent. |
| **INA219 / INA226** | Current monitor | Measure your own power so you can optimize. |
| **LiPo cells (502030, 603048, 103450)** | Flat LiPo | Size to enclosure. |
| **18650 + BMS** | Cylindrical Li-ion | Best energy density for the price. |
| **21700** | Larger cylindrical Li-ion | Higher capacity / current for power-hungry builds. |
| **CR123A / CR2032** | Primary Li | Year-long low-duty sensors. |
| **Supercapacitor (Goldcap)** | Buffer | Graceful shutdown / burst support. |
| **Small solar panel (0.5–6 W)** | Energy harvest | Pair with BQ25504 or MCP73871 for Li charging. |

## Design heuristics

1. **Measure first.** Use an INA219 on your own build to find the 5 mA idle you didn't know you had.
2. **Deep-sleep everything.** An idle ESP32 at ~80 mA vs deep-sleep at ~10 µA is a 10,000× difference.
3. **Wake-from-TPL5110** is cleaner than ULP-on-ESP32 if your interval is > 30 s — lets the ESP32 cut its own regulators.
4. **Separate motor / radio / MCU supplies** — brownouts from actuation are the #1 killer of field builds.
5. **Do not trust fake batteries.** 18650s on AliExpress claiming 9900 mAh are unsafe. Buy from reputable vendors (Samsung, Sony, Panasonic, LG genuine via DigiKey, 18650BatteryStore, IMR Batteries).

## Low-power design tricks

- Use **LoRa + CR123A** for year-long outdoor sensors.
- Run **Wi-Fi modem-sleep** when Wi-Fi is up and idle; saves ~100 mA.
- **Aggressive light-sleep** between events on BLE-only builds.
- **Cut power to sensors** entirely via a P-MOSFET when sleeping — their quiescent draw adds up.
- **Batch cloud uploads** — 1 TX every 15 min beats a trickle.

## Where to buy

- **DigiKey / Mouser** for ICs and protected cells.
- **Adafruit / SparkFun** for pre-built charger breakouts with LiPos sized for hobby.
- **18650BatteryStore / IMR Batteries** for genuine cells.
- **Powerfilm / Voltaic Systems** for solar.
- **AliExpress** for bulk TP4056 and MT3608 (verify counterfeits).

## Gotchas

- **LiPo over-discharge** kills cells; always use a protection board.
- **TP4056** modules often lack protection — read the schematic on the board you bought.
- **Leakage from low-Iq LDOs** is real; verify with a µA-capable current meter (Otii, PPK2, or INA219 in streaming mode).
- **Temperature**: Li chemistry degrades fast above 45 °C — do not mount the battery next to a Peltier or a resistive heater.
