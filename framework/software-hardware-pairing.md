# Software-Hardware Pairing

> **Rule zero.** Every ESP32 build is half a build. The other half is the software on the other side of the wire — the phone, the web dashboard, the LLM prompt, the TinyML model, the CLI, the Figma-tier visualization, the Discord bot. Treat the pair as one product.

## The four software partners

For any hardware build, at least one of these must be designed in from day one.

### 1. On-device software (the embedded twin)

- **Runs on the ESP32 itself.** Arduino, ESP-IDF, MicroPython, Rust (`esp-rs`).
- Uses: real-time actuation, sensor fusion, TinyML inference (TensorFlow Lite Micro, Edge Impulse, ESP-DL).
- When to use: sub-100 ms latency; no network; battery life counts; privacy.
- Pairing pattern: on-device model detects the *event*; off-device model explains or narrates it.

### 2. Edge-local software (the LAN twin)

- **Runs on the same Wi-Fi / Thread / Matter / BLE / ESP-NOW network.**
- Forms: Home Assistant add-on, Node-RED flow, a Python process on a Pi, a web app served from a phone on the LAN.
- Why: keeps asymmetry local, no cloud dependency, fast.
- Pairing pattern: ESP32 publishes MQTT → local broker → local dashboard or local LLM (Ollama / llama.cpp).

### 3. Cloud software (the always-on twin)

- Mobile apps, web dashboards, Supabase / Firebase / custom FastAPI services.
- LLM-driven narration via Claude API, OpenAI API, local-hosted.
- Why: multi-device access, historical archive, collaboration.
- Pairing pattern: ESP32 → HTTPS / WebSocket / MQTT-over-TLS → cloud → persistent store → any-device UI.

### 4. Agentic / LLM software (the reasoning twin)

- An LLM with tools: the ESP32 build *is* one of the tools.
- The LLM can: query the device, correlate across time, suggest actions, generate firmware tweaks.
- Example: "Claude, based on my beehive's last 30 days of data, predict the swarm date and write me a CalDAV invite."
- Pairing pattern: ESP32 exposes a MCP-style / REST tool surface → LLM reasons across streams → issues back-commands or narrates to the human.

## Design heuristic: **the two-line spec**

When designing any build, write exactly two lines before opening your editor:

```
HARDWARE:  <ESP32 variant> + <primary sensor(s)> + <primary actuator/display>
SOFTWARE:  <on-device logic> + <edge or cloud twin> + <human or agent reasoner>
```

Every flagship project in `projects/` starts with these two lines.

## Archetypal pairings (copy these)

| Hardware archetype | Software archetype |
|---|---|
| Wearable IMU + haptic | Mobile app + on-device TinyML gesture classifier |
| Room presence (mmWave + CSI) | Home Assistant + local LLM for "explain my home" |
| Outdoor mesh node (LoRa) | Map dashboard + anomaly-detecting agent |
| Acoustic classifier (MEMS mic) | Edge Impulse model + cloud archive of confident detections |
| E-paper + BLE scanner | Phone app that writes what the badge shows |
| Thermal camera + mmWave | Local vision LLM describing scene in natural language |
| Chemical / VOC stack | Time-series DB + narrating agent "what changed in the air" |
| Bio stack (HRV, SpO2) | Coaching app + LLM journaling companion |

## The unlock: data + software is a force multiplier

One sensor alone is a datum. A sensor + a running LLM prompt + a history is a **witness**. Build witnesses, not data loggers.
