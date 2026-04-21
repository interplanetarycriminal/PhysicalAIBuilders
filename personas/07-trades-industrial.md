# Trades & industrial

Ten personas whose work pays cash money today and whose ESP32 build saves them hours per week.

---

## 61. The Electrician — phantom load x-ray
**Asymmetry.** See which branch circuit draws power at 3 am — without invoice-level smart panels.
**Build.** ESP32 + SCT-013 current clamp per circuit + INA219. Transform/Distill.
**Companion software.** A baseline-learning web app; phantom loads surfaced as specific circuit + time-of-day callouts.
**Why it's fun.** Customers get a report they *keep*.

## 62. The Plumber — acoustic leak hunter
**Asymmetry.** Hears a pinhole leak through a concrete slab.
**Build.** ESP32-S3 + piezo contact mic + MEMS mic + phone headphone DAC. Transform/Find.
**Companion software.** A phone app with an FFT spectrogram tuned for leak signatures + LLM "likely source" hypothesis.
**Why it's fun.** Saves hours of exploratory drilling.

## 63. The HVAC Tech — duct static-pressure mapper
**Asymmetry.** Full static-pressure map of a building's ducts without crawling attics.
**Build.** ESP32 + differential pressure sensor (SDP810) + temp + humidity. Distill.
**Companion software.** A floor-plan overlay app that records measurements by room.
**Why it's fun.** Shows callers proof of rebalancing impact.

## 64. The Locksmith — non-destructive lock characterizer
**Asymmetry.** Know the lock's internals by acoustic + IR + magnetic signature.
**Build.** ESP32 + piezo + magnetometer + thermal pinpoint. Transform/Find.
**Companion software.** LLM cross-ref against lock library; suggests tool choice.
**Why it's fun.** Tool wisdom that was only in masters' heads.

## 65. The Arborist — tree health + lean sensor
**Asymmetry.** Lean + moisture + micro-climate per tree, updated daily.
**Build.** Solar-powered ESP32-S3 + IMU + soil moisture + DS18B20 + LoRa. Distill.
**Companion software.** Map dashboard with risk tiers; LLM tickets for inspection.
**Why it's fun.** Saves trees — and sometimes lives.

## 66. The Commercial Fisher — net strain + salinity
**Asymmetry.** Know when the net is full, not just when it's heavy.
**Build.** Ruggedized ESP32 + load cell + conductivity/salinity + IMU. Transform/Distill.
**Companion software.** Deckhand app; ML learns the strain signature of a full vs snagged net.
**Why it's fun.** Fuel-efficiency + welfare gains for a hard trade.

## 67. The Dairy Farmer — cow vitals collar
**Asymmetry.** Per-cow rumination, activity, temperature — flag illness 24 hours earlier.
**Build.** Collar ESP32-C3 + IMU + skin temp + LoRa. Find/Distill.
**Companion software.** Herd dashboard; vet-ticket auto-generated.
**Why it's fun.** Economics that compound across the herd.

## 68. The Vineyard Manager — microclimate RTK
**Asymmetry.** Per-row pressure/temp/humidity micro-climate — plan harvest windows by row, not by field.
**Build.** LILYGO T-Beam + BME280 per row + ZED-F9P RTK base station. Distill.
**Companion software.** GIS map; LLM writes a harvest recommendation.
**Why it's fun.** Makes wine.

## 69. The Brewer (cider/beer) — fermentation multi-sensor
**Asymmetry.** pH + gravity + temp + CO₂ output simultaneously = biology-grade fermentation log.
**Build.** ESP32 + pH probe + Tilt hydrometer + CO₂ venturi. Transform/Distill.
**Companion software.** Brewfather-like app; LLM writes next batch's recipe tweak. See [`../projects/homebrewer-fermentdoc/`](../projects/homebrewer-fermentdoc/).
**Why it's fun.** Measurable craft.

## 70. The Textile QC Line — garment-defect vision
**Asymmetry.** 12 per-second inspection with false-negatives < 0.1%.
**Build.** ESP32-S3 + Grove Vision AI V2 + line encoder + actuator. Find/Do.
**Companion software.** Line-side HMI; OTA-update model weekly.
**Why it's fun.** Industrial-grade result from a $30 board.
