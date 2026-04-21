# Learning Levels — the magical library

> A reader should **never** feel "lost in the stacks." A great library is one where you can read at any depth — but it nudges you to the *right* shelf for today.

This repo tags every invention, every persona, every flagship project with an **opinionated level from 1 to 100** in increments of 10. A Level 10 build is a Saturday; a Level 100 build is a semester. Pick your rung; climb when you want.

---

## The ladder

| Level | Name | The vibe | Time | Cost | Tools | New skills unlocked |
|---|---|---|---|---|---|---|
| **1** | **Spark** | "Look, it blinks!" | 20 min | $10 | USB cable | Flash an ESP32, read serial. |
| **10** | **First Sense** | One sensor, one display, a delight | 1–2 hrs | $25 | breadboard, Stemma QT | I²C, Arduino IDE, BLE proximity basics. |
| **20** | **Connected Object** | Wi-Fi, phone can read it | 1 evening | $40 | solder-free | HTTP/MQTT, web dashboards, OTA in Arduino. |
| **30** | **Opinionated Firmware** | PlatformIO, Git, a real loop | 1 weekend | $60 | solder iron | FreeRTOS tasks, ESP-IDF components, logging. |
| **40** | **Multi-sensor Distill** | Your build *distills* state | 1 week | $80 | multimeter | sensor fusion, on-device state machines, calibration. |
| **50** | **TinyML on Device** | A model that sees/hears | 2 weeks | $120 | data-labeling workflow | Edge Impulse / TFLite Micro, MFCCs, IMU gesture. |
| **60** | **Mesh & Swarm** | Nodes that coordinate | 2–4 weeks | $150–300 | mesh tooling | ESP-NOW / LoRa / Thread, time sync, multilateration. |
| **70** | **Field Deployment** | It lives outside; it survives | 4–6 weeks | $200–500 | enclosure + battery | KiCad, gasketed enclosures, solar, watchdogs, FOTA. |
| **80** | **Hardware + Agent** | The build is an LLM tool | weeks–months | $300+ | local LLM rig | MCP-style tool surfaces, Claude / Ollama pairing, safety guards. |
| **90** | **Co-processor-Class** | Real compute + AI vision | months | $500+ | desk lab | Grove Vision AI V2 / Coral TPU / Jetson, cross-platform streams. |
| **100** | **Research-grade** | Novel fusion; publishable or ops-ready | semester+ | $$$$ | whole bench | domain partnership, longitudinal data, ethics review. |

Each rung **builds on** the previous, but you can skip rungs if you've already earned the skill elsewhere. The README of each flagship project names the level it is and the level(s) it teaches.

---

## How to use the levels

### As a learner
1. Honest-assess your current level. If you've never flashed an ESP32 → start at Level 1. If you've shipped a battery-powered outdoor build → you're near 70.
2. Pick a persona in [`../personas/`](../personas/) whose *vibe* pulls you. Every persona carries a level tag (see [`personas-by-level.md`](../personas/personas-by-level.md)).
3. The flagship project in [`../projects/`](../projects/) matching that persona's level is your **path**.
4. When the build feels *just* uncomfortable, you're at the right rung.

### As a contributor
When you add a new project or persona, tag its level. If you're not sure, pick the level whose time/cost/skills row best matches. The tag isn't precision-engineering — it's a kindness to the next reader.

---

## What changes between levels

Going from rung to rung, you unlock more of the same five axes:

1. **Parts** — from pre-soldered breakouts to custom PCBs.
2. **Time** — from afternoons to months.
3. **Software twin** — from phone screen, to web dashboard, to agent-driven.
4. **Risk** — from "can't break anything" to "carries ethical implications".
5. **Community pull** — from personal toy to publishable artifact.

When you level up, you usually don't learn one new thing; you learn a *handful* simultaneously, and the blend is the new skill.

---

## The 10 flagship projects as the ladder

Each flagship in [`../projects/`](../projects/) *is* one rung — so you can climb the whole ladder just by building, in order:

| Level | Flagship project | Persona tribe |
|---|---|---|
| 10 | [`party-icebreaker-auralens`](../projects/party-icebreaker-auralens/) | Shadows |
| 20 | [`homebrewer-fermentdoc`](../projects/homebrewer-fermentdoc/) | Domestic |
| 30 | [`ar-garden-oracle`](../projects/ar-garden-oracle/) | Domestic |
| 40 | [`parent-sleep-ally`](../projects/parent-sleep-ally/) | Domestic |
| 50 | [`parkour-flowstate`](../projects/parkour-flowstate/) | Sports |
| 60 | [`falconer-wingcomm`](../projects/falconer-wingcomm/) | Nature |
| 70 | [`beekeeper-hivelink`](../projects/beekeeper-hivelink/) | Nature |
| 80 | [`ghost-hunter-csi-presence`](../projects/ghost-hunter-csi-presence/) | Strange |
| 90 | [`wildfire-lookout-pyrosentinel`](../projects/wildfire-lookout-pyrosentinel/) | High-stakes |
| 100 | [`cold-case-sleuth-timefold`](../projects/cold-case-sleuth-timefold/) | Strange/Knowledge |

Climb in order and you'll have a full Physical-AI Builders skill set by the end.

---

## A quick self-test

Answer "yes / no" to each. The number of yeses ≈ your current level divided by 10.

1. I can flash an ESP32 with the Arduino IDE.
2. I've read an I²C sensor and printed its output.
3. I've built a Wi-Fi-connected device and queried it from my phone.
4. I use PlatformIO or ESP-IDF, not just Arduino IDE.
5. I've calibrated a sensor and fused it with another.
6. I've trained a TinyML model and deployed it.
7. I've run ESP-NOW or LoRa between two nodes I made.
8. I've designed a custom PCB in KiCad and had it fabricated.
9. I've paired a build with a local LLM via a tool-style API.
10. I've deployed a build outdoors for > 30 days unattended.

8/10 → Level 80+. 2/10 → Level 20. No shame anywhere on the ladder.
