# Contributing to PhysicalAIBuilders

This repo is meant to be **riffed on**. Think of every file as a starter prompt.

## Three ways to contribute

### 1. Extend a persona into a full project
Pick one of the 100 entries in [`personas/`](personas/). Copy [`projects/_template/`](projects/_template/) into a new folder named `projects/<slug>/`, then fill in:
- `README.md` — the build story, BOM, wiring, information asymmetry, companion software architecture.
- `firmware/main.cpp` — the ESP32 firmware skeleton (Arduino or ESP-IDF).
- `companion/` — the software that pairs with the hardware (mobile app, web dashboard, CLI, LLM prompt, etc.).

Every flagship project **must** name both its hardware pair and its software pair. No build is complete as silicon alone.

### 2. Add a sensor or add-on
Edit the matching file under [`hardware/sensors/`](hardware/sensors/) or [`hardware/addons/`](hardware/addons/). Include:
- What it measures / does.
- Typical interface (I²C, SPI, analog, UART, USB).
- Which of the seven base capabilities it participates in.
- A "pairs with" note: at least one sensor + one add-on it combines with to create asymmetry.

### 3. Add a creator to the scraper
Edit [`scraper/creators.yaml`](scraper/creators.yaml). Add their channel handle or channel ID and a one-line note on what makes their ESP32 / Physical-AI work distinctive. Then run the scraper to refresh the video index.

## Style

- **Short, dense, vivid.** No filler paragraphs.
- **Name BOTH asymmetries.** Every build should answer: *what does the wielder perceive that others can't?* AND *what meaning does the software assign that others haven't imagined?* See [`framework/information-asymmetry.md`](framework/information-asymmetry.md) §"Two axes" and [`framework/software-as-imagination.md`](framework/software-as-imagination.md).
- **Name the software pair.** No hardware without its software twin.
- **Refuse one-dimensional software.** If your build reads a humidity sensor and calls it a leak, go back and write three more meanings for the same signal (a handshake, a spell, a vote, a heartbeat). Contribute at least one of them as a riff in [`framework/riff-book.md`](framework/riff-book.md).
- **Cite buying sources by vendor name**, not by speculative URLs. The community can keep links current; our job is to be right about what to buy.

## Ethics of asymmetry

Read [`framework/information-asymmetry.md`](framework/information-asymmetry.md) §"Ethics of Asymmetry" before shipping anything that senses people. Defaults: consent, locality, short retention, no covert recording, no stalking vectors.
