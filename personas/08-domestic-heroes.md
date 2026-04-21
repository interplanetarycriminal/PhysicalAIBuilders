# Domestic heroes

Ten personas whose ESP32 build fits in a kitchen drawer and quietly changes a daily moment.

---

## 71. The Overwhelmed Parent — sleep ally
**Asymmetry.** Know the baby's breathing rhythm and air quality from the next room, without a camera.
**Build.** LD2410B + SCD41 + mic + e-paper. Find/Distill/Represent.
**Companion software.** Phone app; LLM summarizes nights into a 2-line text. See [`../projects/parent-sleep-ally/`](../projects/parent-sleep-ally/).
**Why it's fun.** The single highest-love-per-line-of-code build in this repo.

## 72. The Pet Owner — dog-mood monitor
**Asymmetry.** Know when your dog barked, and why (mail vs stranger vs boredom).
**Build.** XIAO ESP32-S3 Sense + mic + IMU collar + camera-base classifier. Find/Reason.
**Companion software.** Weekly "your dog's week" PDF; LLM narrates.
**Why it's fun.** Delightful, calming, surfaces separation-anxiety issues.

## 73. The Elder Caretaker — ambient presence
**Asymmetry.** Know a parent is moving normally in their own home — with zero cameras and zero wearables.
**Build.** LD2410B + DS18B20 on kettle handle + door magnetic + ESP-NOW. Find/Distill.
**Companion software.** Opt-in family group chat gets "Mom's morning routine: complete". No raw data shared.
**Why it's fun.** Preserves elder dignity; relieves adult-child anxiety.

## 74. The Urban Gardener — soil & spectrum oracle
**Asymmetry.** Know which plant needs what before it shows symptoms.
**Build.** ESP32-S3 + soil-moisture array + AS7341 + TSL2591. Transform/Find.
**Companion software.** Phone AR overlay that labels each plant's advice. See [`../projects/ar-garden-oracle/`](../projects/ar-garden-oracle/).
**Why it's fun.** Literal green-thumb augmentation.

## 75. The Homebrewer — fermentdoc
**Asymmetry.** Every batch documented to brewery-grade precision.
**Build.** ESP32 + temp probe + Tilt + CO₂ venturi. Transform/Distill.
**Companion software.** See [`../projects/homebrewer-fermentdoc/`](../projects/homebrewer-fermentdoc/).
**Why it's fun.** Beer/cider you can replicate because you measured it.

## 76. The Insomniac — bedroom environment whisperer
**Asymmetry.** Know which variable (CO₂, RH, sound, light) predicts a bad night.
**Build.** ESP32 + SCD41 + BME280 + mic RMS + TSL2591. Find/Reason.
**Companion software.** Morning report narrating "CO₂ spiked at 03:10 — crack the door tonight".
**Why it's fun.** The quietest possible sleep-research lab.

## 77. The Car-free Commuter — multi-modal journey meter
**Asymmetry.** Know actual door-to-door minutes per option (bike / tram / walk) under actual conditions.
**Build.** XIAO ESP32-C3 + BLE + IMU + GPS. Transform/Distill.
**Companion software.** A phone companion that learns your routes and recommends the right option each morning.
**Why it's fun.** Sharpens a daily decision you already make poorly.

## 78. The Teen Learning to Drive — glance-only trainer
**Asymmetry.** Parent sees overspeeds / hard-brake events summarized at dinner.
**Build.** OBD-II ESP32 dongle + IMU + GPS + LoRa. Find/Distill/Represent.
**Companion software.** Shared dashboard; teen opts in; gamified progress.
**Why it's fun.** Lower parent anxiety + higher teen skill.

## 79. The Marathon Runner — last-10-miles pacer
**Asymmetry.** Haptic rhythm on your wrist that keeps your stride when your brain goes.
**Build.** XIAO ESP32-C3 + IMU + DRV2605 + BLE to phone. Do/Represent.
**Companion software.** Race-day plan loaded to the watch; LLM tweaks pace by HR.
**Why it's fun.** Finishes strong.

## 80. The Dementia Caretaker — gentle wander alert
**Asymmetry.** Know your loved one has stepped outside, in a *gentle*, consent-first way.
**Build.** BLE fob + ESP32 anchors at doors + mmWave zone. Find/Represent.
**Companion software.** Family app; no audio, no video, only threshold events; editable escalation rules.
**Why it's fun.** Love expressed in engineering.
