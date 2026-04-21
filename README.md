# PhysicalAIBuilders

A visionary, playful, and responsible field manual for the **Physical AI Builders community** — where ESP32s, sensors, add-ons, and the unlimited creativity of software combine into projects that give their wielder an **information asymmetry advantage** over the ordinary world.

> **Our thesis.** The ESP32 is the cheapest general-purpose way to teach a patch of the physical world to speak. When you pair one with the right sensors, and pair the sensor data with the right software, you create an information asymmetry: you see, hear, feel, and infer things nobody else in the room can. That asymmetry is the raw material of every great build — from spy gadgets to beekeeper tools to wildfire lookouts to sleep coaches.

## Navigation

| Where to start | Why |
|---|---|
| [`framework/learning-levels.md`](framework/learning-levels.md) | **The 1→100 ladder.** Every invention here is tagged with a level; pick your rung. |
| [`framework/information-asymmetry.md`](framework/information-asymmetry.md) | The core theory: how sensor + ESP32 + software create advantages. |
| [`framework/software-hardware-pairing.md`](framework/software-hardware-pairing.md) | Every build is a hardware-software pair. Here's how to design them together. |
| [`capabilities/`](capabilities/) | The seven base capabilities: **Generate, Transform, Distill, Find, Reason, Do, Represent**. |
| [`hardware/`](hardware/) | ESP32 variants + a full sensor & add-on encyclopedia, indexed by capability. |
| [`personas/`](personas/) | **100 personas** — spies, beekeepers, DJs, parents, ER nurses — each paired with an ESP32 build and the asymmetry it unlocks. Indexed by level in [`personas/personas-by-level.md`](personas/personas-by-level.md). |
| [`projects/`](projects/) | **10 flagship buy/build guides**, each a rung on the ladder (level 10, 20, 30, … 100). Climb in order or skip ahead. |
| [`scraper/`](scraper/) | A **three-stage pipeline**: fetch video catalog → transcribe with YouTube subs + Whisper → synthesize LLM-made learning docs for every video. |

## The magical-library promise

You can enter this repo at any depth. Pick a level in [`framework/learning-levels.md`](framework/learning-levels.md) and the rest of the site sorts itself to that shelf. Every page teaches at that level without overwhelming you, and stairs are clearly marked when you're ready to climb.

## The seven base capabilities

Every Physical-AI build maps to one or more of these verbs:

1. **Generate** — produce new data (images, sound, motion, light, text) from prompts or state.
2. **Transform** — reshape data into a different representation (raw accel → gesture, audio → transcript, image → depth).
3. **Distill** — compress many signals into one salient insight ("air quality is risky from 14:00–17:00").
4. **Find** — locate patterns, anomalies, objects, or people in noisy streams.
5. **Reason** — infer causality, intent, or next action across modalities.
6. **Do** — actuate the world (motors, relays, IR, speakers, DMX, solenoids).
7. **Represent** — externalize internal state to humans or other agents (dashboards, haptics, e-paper, AR).

The canonical informal definition: **"take some data in, and either change it, make new data from it, fire actions in response to what the data means, or grant the receiver an information asymmetry advantage."**

## How to contribute

Read [`CONTRIBUTING.md`](CONTRIBUTING.md). Then riff on a persona, extend a sensor page, or build a new flagship project using the template in [`projects/_template/`](projects/_template/).

## License & tone

Everything here is published with a builder-first, curiosity-first, ethics-forward tone. Information asymmetry is power — use it to help, delight, and expand agency, not to surveil or harm. See [`framework/information-asymmetry.md`](framework/information-asymmetry.md) §"Ethics of Asymmetry".
