# The Theory of Information Asymmetry for Physical AI Builders

## The one-line version

> An ESP32 build is valuable exactly to the degree that it lets its wielder **perceive, infer, or act on** something that the rest of the room cannot.

That gap — between what you know and what others know — is an **information asymmetry**. Every great Physical-AI build is a machine for manufacturing one.

## Where asymmetries come from

A sensor is a lens onto a layer of reality that humans don't have cheap access to. Stack three lenses on a $4 microcontroller, run a little software on top, and you get superpowers.

| Asymmetry class | Example sensor stack | The superpower it grants |
|---|---|---|
| **Temporal** — you see it earlier | LD2410B mmWave + INMP441 mic | Breathing anomaly detected before the baby cries |
| **Spatial** — you see it elsewhere | LoRa SX1262 + GPS + IMU | Know a falcon's altitude from 4 km away |
| **Spectral** — you see outside visible light | MLX90640 thermal + AS7341 spectral | See hot stud behind drywall; diagnose plant nutrient by leaf reflectance |
| **Ambient-RF** — you read the invisible | ESP32 Wi-Fi CSI + BLE sniff | Presence and gait through walls without a camera |
| **Multi-modal fusion** | BME680 + CO2 + PM2.5 + mic | Air-quality event classified as "cooking" vs "wildfire" vs "HVAC fault" |
| **Crowd-aggregate** | A mesh of the above | See the pattern *between* locations, not just at one |
| **Historical/derivative** | Any sensor + local storage + small model | You have the memory nobody else kept |
| **Counter-factual** | Sensor stack + simulation | "What would normal look like right now?" → deviation is the signal |

## The asymmetry loop

```
  sense  →  transform  →  distill  →  represent
    ↑                                      ↓
   act  ←───  reason  ←────  find  ←───  (human or agent)
```

Every flagship project in `projects/` traces this loop explicitly.

## Software is the multiplier, not the afterthought

A thermal camera alone tells you temperatures. A thermal camera with a companion LLM prompt that asks "is this a water leak, a rodent, a person, or a hot-water pipe?" tells you **what to do next**. Pairing hardware with software is non-optional: see [`software-hardware-pairing.md`](software-hardware-pairing.md).

Software extends asymmetry in four directions:
1. **Inference** — tiny models on-device (TinyML) or big models off-device (LLM, vision transformer).
2. **Memory** — you keep a log; you query "when was the last time…".
3. **Comparison** — you compare two places, two times, two people, two runs.
4. **Narration** — you convert a scalar stream into a human-legible story or alert.

## Ethics of asymmetry

Asymmetry is power, and power requires care. The community defaults:

- **Consent by default.** If a sensor can perceive a person, that person should be able to discover and opt out. Put an LED on every microphone; expose a visible BLE beacon you can verify with your phone.
- **Local-first.** Raw sensor data stays on device or on the LAN. Cloud is a privilege, not a default.
- **Short retention.** Raw streams expire; only distilled insights persist.
- **No stalker kit.** Do not build for covert tracking of a specific person without their active consent. This community will not host such projects.
- **Transparency of the asymmetry.** Your project README should state *who knows what that others do not*. If you can't write that sentence without discomfort, rethink the build.
- **Reversible.** Every build should have a kill switch — a hardware button or a flashed-default state — that returns it to "harmless blinking light".

Build like a guest in the world, not an occupier.

## How to read the rest of the repo through this lens

- [`capabilities/`](../capabilities/) gives you the verbs.
- [`hardware/`](../hardware/) gives you the nouns.
- [`personas/`](../personas/) gives you the *subjects* — whose asymmetry are we amplifying?
- [`projects/`](../projects/) shows you complete, shippable asymmetry machines.

Go build one.
