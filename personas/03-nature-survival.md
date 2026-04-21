# Nature & survival

Ten personas whose subject is the non-human world — and ten asymmetries that help them listen to it without disturbing it.

---

## 21. Steve Irwin — reptile vitals telemetry (ethical version)
**Asymmetry.** Know a large reptile's resp rate, skin temp, and micro-climate without touching it after the first placement.
**Build.** ESP32-C3 LoRa collar + MAX30208 skin-temp + piezo resp-belt + BME280. Find/Distill.
**Companion software.** A keeper's dashboard with "stress up" alerts; LLM narrates daily welfare summary.
**Why it's fun.** Cheap collars save big-animal welfare studies from budgetary death.

## 22. Bear Grylls — wilderness survival beacon
**Asymmetry.** When you're lost, you still have HR, temp, pressure/altitude, and a LoRa SOS that reaches 10 km.
**Build.** LILYGO T-Beam (ESP32 + LoRa + GPS) + MAX30102 + BMP390. Represent/Do.
**Companion software.** Meshtastic network; an LLM on a ranger station that parses incoming beacons for distress patterns.
**Why it's fun.** Something you hope you never need — and are glad you had when you did.

## 23. The Falconer — falcon telemetry glove
**Asymmetry.** Knows the bird's altitude, speed, heading *and* can see its GoPro via a dedicated 2.4 GHz link.
**Build.** Bird: tiny ESP32-C3 + GPS + BMP + LoRa. Glove: LILYGO T-Display + LoRa + haptic. Find/Represent.
**Companion software.** An "altitude + heading + 'has killed' state" overlay; local map tile cache for offline.
**Why it's fun.** An ancient sport gains a honest, respectful telemetry layer. See [`../projects/falconer-wingcomm/`](../projects/falconer-wingcomm/).

## 24. The Beekeeper — hive acoustic-weight-temp monitor
**Asymmetry.** Predict swarms days in advance by hive weight + acoustic tonal shift.
**Build.** ESP32-S3 + HX711 load cell under hive + piezo in hive wall + BME280 + DS18B20 array. Transform/Distill/Find.
**Companion software.** A web dashboard with forecasted swarm date; LoRa uplink to apiary HQ. See [`../projects/beekeeper-hivelink/`](../projects/beekeeper-hivelink/).
**Why it's fun.** Ancient craft meets microphone-learned insect behavior.

## 25. The Storm Chaser — pressure + lightning + wind triangulator
**Asymmetry.** A mesh of three cars knows exactly where the cell is forming.
**Build.** LILYGO T-Beam + AS3935 lightning detector + BMP581 + anemometer. Find/Reason.
**Companion software.** Map app that triangulates lightning strikes, fuses with METAR + HRRR.
**Why it's fun.** The community mesh is the product.

## 26. The Mycologist — mushroom spore & spectrum lab
**Asymmetry.** Identify mushrooms & their micro-habitat — spore density, humidity, spectral reflectance of caps.
**Build.** ESP32-S3 + AS7341 + SHT45 + tiny particle sensor. Transform/Find.
**Companion software.** LLM-assisted ID that combines reflectance + morphology image + habitat data.
**Why it's fun.** The craft of foraging is meeting the craft of citizen science.

## 27. The Coral Restoration Diver — reef-minder
**Asymmetry.** Underwater pH, temp, light, current direction — for each fragment.
**Build.** ESP32 in potted enclosure + EZO pH + DS18B20 + TSL2591 + water-flow sensor. Transform/Distill.
**Companion software.** A dive log that auto-tags dives with the reef's chemistry.
**Why it's fun.** Combines field conservation with beautiful data.

## 28. The Rewilder — species soundscape listener
**Asymmetry.** Counts & IDs species acoustically across 24 hours.
**Build.** ESP32-S3 + INMP441 + 18650 + solar + LoRa. Find/Distill.
**Companion software.** Edge Impulse bird-classifier + a Claude-written weekly "who visited the hedge" report.
**Why it's fun.** Your property becomes a reserve you *know*.

## 29. The Shepherd — LoRa herd collars
**Asymmetry.** Know where each of 120 sheep is, and which ones aren't moving normally.
**Build.** LILYGO T-Beam or small custom LoRa + GPS + IMU + solar. Find/Distill.
**Companion software.** Map + anomaly (one sheep hasn't moved in 4 hrs → go check) + a grazing-pattern pasture-rotation advisor.
**Why it's fun.** A farm tool that also scales to alpine + outback use.

## 30. The Fire Lookout — ridge-top sentinel
**Asymmetry.** Smoke / PM / thermal plume direction + wind shift — hours before the nearest human reports.
**Build.** ESP32-S3 + MLX90640 + SEN55 + BME280 + anemometer + LoRa. Find/Distill.
**Companion software.** Regional fire-ops dashboard; see [`../projects/wildfire-lookout-pyrosentinel/`](../projects/wildfire-lookout-pyrosentinel/).
**Why it's fun.** One of the highest-stakes asymmetries you can build.
