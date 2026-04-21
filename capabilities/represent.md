# Represent

> Externalize internal state in a form a human or another agent can consume — glyphs on a screen, color on an LED, haptic pattern on the wrist, voice in the ear, structured JSON on the wire.

## In this repo's vocabulary

Represent is the *make-legible* verb. It is the output face of the asymmetry loop: whatever Find / Transform / Distill / Reason computed, Represent is the moment it enters the human's (or next agent's) mental model.

A good Represent is **ambient when calm, crisp when urgent, and silent when nothing matters**.

## Classic ESP32 implementations

### On-device visual
- **E-paper** — GDEY/Waveshare 2.9–7.5" panels; perfect for "one sentence per hour" distilled output.
- **OLED** — SSD1306 (128×64), SSD1327 (128×128 grayscale); great status displays.
- **TFT/IPS** — ST7789 (240×240/240×320), LVGL UI framework; for rich UI.
- **NeoPixel** — state via color+pattern; iconic for ambient display.

### On-device non-visual
- **Haptic** — LRA + DRV2605; morse-code for wearables, heart-rate-sync hugs in remote presence.
- **Sonification** — an I²S speaker playing a timbre that maps to a scalar (pitch = CO2, for example).
- **Thermal printing** — a receipt handed to you; the most satisfying "the machine made me something" affordance.

### With companion software
- **Web dashboards** — MQTT → local broker → web app on phone.
- **Native apps** — BLE-connected iOS/Android.
- **AR overlays** — ESP32 beacons the phone discovers; phone paints the invisible.
- **Voice narration** — TTS from an LLM reads out the distilled state on demand.
- **Other agents** — Represent a build's state as a structured tool-surface that another LLM consumes.

## Sensors & add-ons that feed Represent

Represent consumes outputs from the other verbs. But it *creates* the user experience — so the add-on chosen here defines the product's feel:

- E-paper says "calm, archival, read once".
- NeoPixel says "ambient, glanceable, pulse".
- Haptic says "private, urgent, embodied".
- Thermal paper says "take this with you".
- AR overlay says "layer on top of reality".

Choose your Represent add-on **for the emotional contract** as much as the information.

## Failure modes

- **Alert fatigue** — every representation competes with the others in the user's life. Budget each build's attention use.
- **Mode confusion** — the same color means different things in two different builds. Adopt a house style.
- **Silent failure** — if the Represent channel breaks, the whole system looks "fine". Always include a heartbeat glyph.

## Cross-references

- Personas: **mime** (haptic-only), **librarian** (e-paper map), **DJ** (audience-LED feedback), **elder caretaker** (ambient color), **teen driver** (glance-only).
- Flagship projects: [`projects/party-icebreaker-auralens/`](../projects/party-icebreaker-auralens/), [`projects/ar-garden-oracle/`](../projects/ar-garden-oracle/), [`projects/falconer-wingcomm/`](../projects/falconer-wingcomm/).
