# PhysicalAIBuilders

A visionary, playful, and responsible field manual for the **Physical AI Builders community** — where ESP32s, sensors, add-ons, and the unlimited creativity of software combine into projects that give their wielder an **information asymmetry advantage** over the ordinary world.

> **Our thesis.** The ESP32 is the cheapest general-purpose way to teach a patch of the physical world to speak. When you pair one with the right sensors, and pair the sensor data with the right *imaginative* piece of software, you create an information asymmetry on **two** axes: you **see** things nobody else in the room can, AND you **decide what those signals mean** in ways nobody else has imagined. A capacitive humidity sensor touched by a finger reports a "water value" — in ordinary software that's a leak; in creative software it's a handshake between two cities, a spell in a game, a vote in a gallery, a heartbeat in a story. Same hardware. Different genre. That second axis is **unbounded**, and most of the fun lives there.

> **Read order:** start at [`framework/learning-levels.md`](framework/learning-levels.md) to pick your rung, then [`framework/information-asymmetry.md`](framework/information-asymmetry.md) for the two axes, then [`framework/software-as-imagination.md`](framework/software-as-imagination.md) for what software *can* do once you stop thinking one-dimensionally.

## Navigation

| Where to start | Why |
|---|---|
| [`framework/learning-levels.md`](framework/learning-levels.md) | **The 1→100 ladder.** Every invention here is tagged with a level; pick your rung. |
| [`framework/information-asymmetry.md`](framework/information-asymmetry.md) | The core theory: two axes of asymmetry — **perception** *and* **meaning**. |
| [`framework/software-as-imagination.md`](framework/software-as-imagination.md) | **Read this second.** The software layer is where unlimited creativity lives; a humidity sensor touched by a finger can be a handshake, a spell, a heartbeat in a story. Twelve modes of software imagination. |
| [`framework/riff-book.md`](framework/riff-book.md) | 60+ sensor-event → creative-software pairings. The community's living gallery. |
| [`framework/software-hardware-pairing.md`](framework/software-hardware-pairing.md) | Every build is a hardware-software pair. Here's how to design them together. |
| [`capabilities/`](capabilities/) | The seven base capabilities: **Generate, Transform, Distill, Find, Reason, Do, Represent**. |
| [`hardware/`](hardware/) | ESP32 variants + a full sensor & add-on encyclopedia, indexed by capability. |
| [`personas/`](personas/) | **110 personas** — spies, beekeepers, DJs, parents, ER nurses, plus a [creative-remix tribe](personas/11-creative-remix.md) where the entire asymmetry is software imagination. Indexed by level in [`personas/personas-by-level.md`](personas/personas-by-level.md). |
| [`projects/`](projects/) | **11 flagship buy/build guides** — 10 on the ladder (L10 → L100) plus the creative flagship [`pirate-stage-walkby`](projects/pirate-stage-walkby/) where walking past a hallway camera turns you into a pirate on rough seas on a projected wall. |
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
