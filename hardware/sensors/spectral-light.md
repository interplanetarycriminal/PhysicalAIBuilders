# Spectral & light sensors

Visible light (lux), color, UV, multispectral (IR/NIR bands). The "what does the light look like?" stack.

## Part catalog

| Part | Measures | Interface | Note |
|---|---|---|---|
| **TSL2591** | Broadband + IR lux (high dynamic range) | I²C | 188 µlux to 88 klux — moonlight to sun. |
| **BH1750** | Lux | I²C | Cheap, reliable ambient light. |
| **VEML7700** | Lux | I²C | Modern replacement for BH1750; better noise. |
| **VEML6075** | UV-A / UV-B | I²C | Sun exposure tracking. |
| **LTR390** | UV + ALS | I²C | Modern UV index + lux in one. |
| **TCS34725** | RGB color | I²C | Classic color sensor. |
| **APDS-9960** | Color, proximity, gesture | I²C | Touchless gesture + color + proximity. |
| **AS7341** | 11-channel spectral | I²C | Visible+NIR spectroscopy — plant health, skin tone, water clarity. |
| **AS7263 / AS7265x** | 6/18-channel NIR | I²C | NIR spectroscopy for plastic-ID and tissue analysis. |
| **SI1145** | UV / IR / visible | I²C | One chip; deprecated but still useful. |

## Interface notes

- Many of these sensors saturate quickly. Configure gain and integration time *dynamically* — the library defaults are for classrooms.
- Spectral sensors (AS7341) need an even-illumination source; use a white LED that you characterize.

## Capability contribution

- **Find**: anomalies in light (flicker, hue shift), spectral fingerprint match (plastic type, mineral, tissue).
- **Transform**: spectrum → reflectance vector → classifier input.
- **Distill**: daily lux exposure → circadian health signal.
- **Reason**: lux + time + location → "you didn't get enough outdoor light today".
- **Represent**: spectral bar-graph TFT, circadian NeoPixel ring.

## Pairs with (asymmetry combos)

- **AS7341 + phone camera + LLM** — plant health scanner. Spectrum → embedding → VLM narration: "nitrogen deficient, leaf age mid, watering OK".
- **TSL2591 + circadian NeoPixel** — a wearable that nags you to go outside at 10 am.
- **APDS-9960 + OLED** — touchless menu navigation on a kitchen appliance.
- **AS7265x + TFT + ESP32-S3** — counterfeit currency / material verification device.
- **UV sensor + e-paper** — wearable today's-UV-dose badge.

## Where to buy

- **Adafruit / SparkFun** for TSL2591, VEML7700, VEML6075, LTR390, APDS-9960, AS7341 breakouts.
- **DigiKey / Mouser** for ams-OSRAM AS7341 / AS7265x bare parts.

## Gotchas

- **Cover glass matters** for UV sensors — most window glass blocks UV-B entirely.
- **IR leakage** into ALS channels biases lux in incandescent light; use the TSL2591's IR channel to compensate.
- **Spectral sensors** are comparative instruments unless you also measure a white reference tile; build a calibration protocol.
