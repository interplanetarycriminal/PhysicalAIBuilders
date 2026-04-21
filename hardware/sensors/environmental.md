# Environmental sensors

Temperature, humidity, barometric pressure, volatile organic compounds (VOCs), particulate matter (PM1/2.5/10), CO₂, NOx. The air-quality stack.

## Part catalog

| Part | Measures | Interface | Note |
|---|---|---|---|
| **BME280** | Temp, humidity, pressure | I²C / SPI | The safe default; accurate, well-supported. |
| **BME680** | + VOC / gas-resistance | I²C / SPI | Adds a metal-oxide gas sensor; Bosch IAQ library distills to one score. |
| **BME688** | + 8-class AI gas classification | I²C | Successor to 680; trainable gas class (bread vs coffee vs smoke). |
| **SHT41 / SHT45** | Precise temp + humidity | I²C | Lab-grade. Pair with a BME280 for pressure. |
| **DS18B20** | Temp only | 1-Wire | Waterproof probe versions for bees / brews / soil. |
| **MAX31855 + type-K TC** | High-temp (−200 to +1300 °C) | SPI | Kilns, forges, smokers, boilers. |
| **SCD40 / SCD41** | CO₂ (NDIR), temp, humidity | I²C | True NDIR CO₂, not VOC-proxy. Essential for indoor air. |
| **SCD30** | CO₂ (NDIR), temp, humidity | I²C | Slightly older, still excellent. |
| **SGP40 / SGP41** | VOC + NOx index | I²C | Sensirion's VOC index is a tuned distillation, more stable than raw. |
| **ENS160** | VOC / eCO₂ | I²C | ScioSense; cheap companion to BME280. |
| **SEN54 / SEN55** | PM1/2.5/4/10 + VOC + NOx (+RH/T) | I²C | One module, the whole air-quality stack. |
| **PMS5003 / PMS7003** | PM1/2.5/10 (laser scatter) | UART | Cheap, reliable PM — the backbone of many citizen-air-quality projects. |
| **HTU31** | Temp + humidity | I²C | Fast response; good for HVAC studies. |
| **LM35 / LM75** | Temp | Analog / I²C | Old-school cheap probes. |

## Interface notes

- Keep I²C buses short and shielded. >30 cm without bus drivers gets flaky.
- UART-based PM sensors (PMS5003) draw real current (~100 mA during fan operation) — budget for it.
- CO₂ NDIR sensors (SCD4x) need ~2s warmup; they're not instant.

## Capability contribution

- **Transform**: raw VOC resistance → IAQ score.
- **Distill**: a day of PM2.5 → "don't exercise outside after 16:00".
- **Find**: spikes in CO₂ locate people; VOC spikes locate cooking/cleaning events.
- **Reason**: combine PM + CO₂ + VOC to classify the kind of event (cooking vs smoke vs HVAC fault).
- **Represent**: color-coded LED, ambient display, narration via LLM.

## Pairs with (asymmetry combos)

- **BME688 + microphone** — classify cooking event *and* hear the pan sizzling for cross-modal confidence.
- **SEN55 + mmWave presence** — air event is meaningful only when occupants are home.
- **SCD41 + mic + IMU** — detect that *you* are holding your breath while debugging (bio-ambient fusion).
- **BME280 + GPS + LoRa** — distributed pressure map of a hillside, predict microclimate shifts for a vineyard.

## Where to buy

- **DigiKey / Mouser** for Sensirion, Bosch, ScioSense modules (reliable genuine parts).
- **Adafruit / SparkFun / Pimoroni** for STEMMA QT / Qwiic breakouts — no soldering.
- **Seeed Studio** for Grove versions.
- **AliExpress** for volume — watch for fake BME280s (they're everywhere; genuine ones read humidity properly at low T).

## Gotchas

- **Self-heating**: the BME680 heats the gas sensor; always sample ambient temperature from a *separate* sensor if you care about T accuracy.
- **Fake BME280 modules** report implausible humidity — test a known-good humidity source before trusting.
- **NDIR warmup** on SCD4x: don't alarm on the first minute of data.
- **PM sensors hate humidity > 95%** — droplets scatter like particles and drive false PM readings. Gate alarms on RH.
