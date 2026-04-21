# Sci-fi tech geniuses & gadgeteers

Ten personas from the fiction that made us want to build.

---

## 11. Tony Stark — wrist HUD
**Asymmetry.** Ambient situational awareness of the room: presence, heat, air, people named.
**Build.** LILYGO T-Watch S3 + LD2410B (pocket) + MLX90640 (pocket) + INMP441. Find/Transform/Represent.
**Companion software.** Phone-pocket hub running local Ollama + Home Assistant, narrating the room into the watch via BLE.
**Why it's fun.** The closest thing to a JARVIS you can actually solder this weekend.

## 12. Hiro Hamada (Big Hero 6) — microbot swarm
**Asymmetry.** A fistful of tiny bots coordinate to map and navigate a room.
**Build.** 20× XIAO ESP32-C3 + IMU + vibration motor per bot; ESP-NOW mesh. Find/Do/Represent.
**Companion software.** Web app that watches the mesh, aggregates position guesses via multilateration, and drives the swarm with one command.
**Why it's fun.** Buy 20 boards for $100; the first time they move as one, you'll yell.

## 13. Doc Brown — flux-capacitor dashboard
**Asymmetry.** Time-travel vibe: show the *derivative* of every sensor, not its value.
**Build.** ESP32-S3 + ST7789 IPS + array of sensors on I²C (BME680, MPU6050, MAX30102). Transform/Represent.
**Companion software.** LVGL dashboard that only renders *rate of change* — nothing stays still, which is the point.
**Why it's fun.** Aesthetic. Reveals signals invisible to normal dashboards.

## 14. MacGyver — any-sensor-to-any-function kit
**Asymmetry.** Whatever you have, something useful can be built.
**Build.** ESP32 + a labelled bag of every cheap sensor you could hot-glue. Transform/Do.
**Companion software.** A CLI "recipe book" where an LLM is given the list of sensors you have and proposes three builds.
**Why it's fun.** Turns procrastination into progress — and the LLM is a great pair-programmer for improvisation.

## 15. Neo — perception badge
**Asymmetry.** You see the Matrix: RF activity, Wi-Fi saturation, BLE density, CO₂ rising.
**Build.** ESP32-S3 + small green OLED + LoRa receiver in sniff mode + mic. Find/Represent.
**Companion software.** Phone view with a "what the room is actually doing" stream, styled as falling green glyphs.
**Why it's fun.** The only build that feels like a superpower activation animation.

## 16. Rick Sanchez — omni-instrument
**Asymmetry.** All the data, all the time. Organized badly on purpose.
**Build.** ESP32-P4 + ESP32-C6 combo + every sensor category in [`../hardware/sensors/`](../hardware/sensors/) at once. Find/Transform/Distill.
**Companion software.** A chaotic but useful Jupyter-style live dashboard + a Claude companion named "Morty" that patiently explains what each graph means.
**Why it's fun.** The builder's equivalent of a tortured genius's desk. Makes excellent YouTube content.

## 17. Shuri — Vibranium monitor
**Archetype.** Material scientist + tech prodigy.
**Asymmetry.** Characterize an unknown object by its spectral, vibration, and acoustic signature.
**Build.** ESP32-S3 + AS7341 + piezo + MEMS mic + AD8232 (as EMF-analog sniffer). Transform/Find/Reason.
**Companion software.** An LLM-backed "what is this?" that cross-references a learned library of materials.
**Why it's fun.** Genuinely new identification capability in your pocket.

## 18. Iron Giant — companion body-tracker
**Asymmetry.** The giant knows where its human is, always.
**Build.** Two ESP32-S3 nodes — one wearable (IMU, BLE) and one stationary — UWB between them. Find/Represent.
**Companion software.** A small game where the giant's state (on e-paper) reflects where the child is in the house.
**Why it's fun.** Emotional design-led build — a toy that *misses you*.

## 19. WALL-E — autonomous trash sorter
**Asymmetry.** Cameras + scale + AI that sorts recyclables correctly — better than humans do.
**Build.** ESP32-CAM + HX711 load cell + 3× servos + Grove Vision AI V2. Find/Transform/Do.
**Companion software.** A local training loop: you label mistakes on a phone app; the model improves weekly.
**Why it's fun.** A daily, tangible climate win. Also meltingly cute if housed in a WALL-E shell.

## 20. Q-Branch Engineer (movies) — the rapid prototyping brief
**Asymmetry.** Can prototype any imagined device in 48 hours.
**Build.** Unexpected Maker FeatherS3 + a curated "briefcase" of 20 sensors + magnetic solderless wiring. Do/Represent.
**Companion software.** A project template that ingests a brief ("build me a X") and scaffolds firmware + BOM + wiring diagram via an LLM.
**Why it's fun.** This IS the meta-tool. Probably your second favorite build.
