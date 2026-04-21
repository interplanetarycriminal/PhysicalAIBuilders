# Generate

> Produce new data — images, sound, light, motion, text — from prompts, environmental state, or internal reasoning.

## In this repo's vocabulary

Generate is the *creative* verb. The output doesn't just describe or react; it is new content brought into the world by the device. Generation can be rule-based, procedural, model-based, or LLM-orchestrated.

## Classic ESP32 implementations

### On-device
- **Audio synthesis** — sine/FM/wavetable via I²S DAC (MAX98357A, PCM5102). Good for beeps, timbral cues, drone pads.
- **Light patterns** — procedural NeoPixel animations (rainbow, perlin-noise, audio-reactive, biometric-reactive).
- **Micro text** — templated strings rendered to OLED/e-paper; enough for status, haiku, fortune-cookie outputs.
- **Motion generation** — servo sequences choreographed to audio beat or sensor events.
- **Procedural visuals** — 240×240 TFT or RGB LED matrix with Lissajous / particle sim / reaction-diffusion.

### With companion software
- **TTS from an LLM** — ESP32 sends state → cloud/Edge LLM → streams PCM audio back to ESP32's I²S DAC.
- **Generative art** — ESP32 sensor state seeds a Stable-Diffusion / local diffusion run; result e-paper printed or displayed.
- **Agentic generation** — Claude/LLM receives telemetry, generates a short personalized haiku, sends it to e-paper.

## Sensors & add-ons that feed Generate

- **Audio**: MEMS mic (INMP441) — ambient sound seeds generative music.
- **Bio**: HRV (MAX30102), GSR — generate visuals/sound tuned to physiological state.
- **Environmental**: BME680 — generate a daily "weather haiku" from pressure & VOC trends.
- **Add-ons**: I²S DAC, NeoPixel, RGB LED matrix, servos, thermal printer, e-paper.

## Failure modes

- **Sampling-rate drift** in I²S — buffer underruns audible as clicks; fix with proper DMA ring buffers.
- **NeoPixel data-line timing** interrupted by Wi-Fi — use RMT peripheral or a separate core (ESP32 has two).
- **Generative monotony** — without input variety, output gets boring fast; always seed with sensor state.

## Cross-references

- Personas: **DJ `bpm-sympath`**, **haiku poet**, **kinetic sculptor**, **homeschool parent**.
- Flagship project: [`projects/dj-bpm-sympath/`](../projects/dj-bpm-sympath/), [`projects/homebrewer-fermentdoc/`](../projects/homebrewer-fermentdoc/).
