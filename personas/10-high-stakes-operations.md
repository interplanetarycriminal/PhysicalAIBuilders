# High-stakes operations

Ten personas whose decisions affect lives. Ethics of asymmetry (see [`../framework/information-asymmetry.md`](../framework/information-asymmetry.md)) apply hardest here.

---

## 91. The Wildfire Incident Commander — PyroSentinel
**Asymmetry.** Ridge-top PM + thermal + wind + mesh across 50 km².
**Build.** See [`../projects/wildfire-lookout-pyrosentinel/`](../projects/wildfire-lookout-pyrosentinel/).
**Companion software.** Ops dashboard + LLM that drafts SitReps for the team.
**Why it's fun (really: meaningful).** Hours of lead time on evacuation decisions.

## 92. The Search-and-Rescue Diver — acoustic tether
**Asymmetry.** Silent ultrasonic comms + through-water location between divers + surface.
**Build.** Potted ESP32 + ultrasonic transducer + IMU + depth. Do/Represent.
**Companion software.** Surface command tablet with dive locations + biometrics.
**Why it's fun.** Lifesaving infrastructure built in a kitchen workshop.

## 93. The Avalanche Forecaster — snowpack watcher
**Asymmetry.** Multi-layer snowpack temp + settlement + stability per slope.
**Build.** Buried stake with ESP32 + DS18B20 array + piezo + load cell + LoRa. Find/Distill.
**Companion software.** Ski-patrol dashboard + LLM SitReps.
**Why it's fun.** Cheap, dense data where none existed.

## 94. The ER Charge Nurse — waiting-room triage
**Asymmetry.** Consent-indicated ambient sensing (skin temp, resp rate, HR via mmWave) of the waiting room.
**Build.** ESP32-S3 + LD2450 + AMG8833 + mic (RMS only). Find/Distill.
**Companion software.** Triage view highlighting who's deteriorating; opt-in, visible LED.
**Why it's fun (meaningful).** Saves exactly the people the system misses.

## 95. The Refugee Aid Worker — multilingual badge
**Asymmetry.** Live translation + situational audio capture (consent-gated) on a wearable.
**Build.** XIAO ESP32-S3 Sense + mic + speaker + BLE to phone. Transform/Generate.
**Companion software.** Whisper + LLM translation; offline mode mandatory.
**Why it's fun.** Dignity & communication in the toughest moments.

## 96. The Drone-forward Surveyor — UWB + RTK grid
**Asymmetry.** Centimeter-accurate ground truth for drone photogrammetry.
**Build.** 4× ESP32-S3 UWB anchors + ZED-F9P base + drone tag. Find.
**Companion software.** Live overlay in DJI SDK / custom Pilot app.
**Why it's fun.** Professional kit for hobby money.

## 97. The Precision-ag Farmer — swarm scouts
**Asymmetry.** 1 scout per hectare, soil + spectral + moisture.
**Build.** Solar ESP32-S3 stakes + soil moisture + AS7341 + LoRa. Distill.
**Companion software.** Farm map; LLM writes daily "fertigate this zone" directives.
**Why it's fun.** Capex → opex win for any farm.

## 98. The Data Center Thermal Engineer — rack sentinel
**Asymmetry.** Per-U thermal gradient + vibration (fan health) + airflow.
**Build.** ESP32 + BME + MLX90614 array + piezo per rack + Ethernet via W5500. Distill.
**Companion software.** Rack-wise dashboard; LLM proposes cooling rebalancing.
**Why it's fun.** Dollar-valuable in any DC.

## 99. The Negotiator — micro-expression coach
**Asymmetry.** Quiet earpiece coaching from a second party who sees pulse, voice tempo, breath.
**Build.** Discreet MAX30102 + mic + bone conduction + LoRa. Distill/Do.
**Companion software.** Coach's phone view; LLM flags "their stress just spiked" moments.
**Why it's fun.** The most ethically loaded build here — consent of all parties *before you build it*.

## 100. The Disaster Comms Lead — Meshtastic command post
**Asymmetry.** Hundreds of LoRa nodes self-organize into a text backbone when cellular is down.
**Build.** Heltec / LILYGO LoRa ESP32 nodes + GPS. Represent/Do.
**Companion software.** Command laptop running a Meshtastic gateway + an LLM composing clean SitReps from terse field messages.
**Why it's fun (necessary).** A gift to communities in crisis.
