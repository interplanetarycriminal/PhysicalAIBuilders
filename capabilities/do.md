# Do

> Actuate the world. The moment a Physical-AI build becomes physical.

## In this repo's vocabulary

Do is the *effect* verb. Every other verb in this repo is digital, mostly; Do is where electrons become motion, heat, light, sound, radio energy, or a printed paper receipt. Do is also where the safety surface is largest — so we guard it hardest.

## Classic ESP32 implementations

### On-device
- **PWM actuation** — servos, DC motors via H-bridge, LED dimming.
- **Stepper control** — A4988 / DRV8825 / TMC2209 silent-step drivers.
- **Solenoids & relays** — MOSFET-driven or optoisolated relay boards.
- **IR blasters** — `IRremoteESP8266` library; control TVs, ACs, garage doors, cameras.
- **Speakers** — I²S DAC (MAX98357A) → amplified sound, voice prompts.
- **DMX over RS-485** — stage lighting integration with MAX485 level shifter.
- **Thermal printing** — classic 58mm receipt printer for e-paper-free output.
- **Haptic** — DRV2605 haptic driver + LRA for wearables and badges.

### With companion software
- **Matter / HomeKit bridges** — ESP32 exposes switches/sensors to existing smart-home ecosystems.
- **Robotic arm orchestration** — ESP32 as joint-level controller; higher-level path planning on an edge box.
- **Agent-initiated actions** — LLM decides; ESP32 enforces *physical* safeties.

## Sensors & add-ons that feed Do

Do consumes *decisions*, not sensor readings. But every Do must be paired with:
- A **corroborating sense** — confirm the actuation happened (limit switch on a servo arm, current draw on a motor).
- A **reversibility mechanism** — a big, labeled kill switch on anything that moves.

## Safety rules of thumb

1. **Inrush current** — motors/solenoids spike 5–10× steady-state; size your MOSFETs and supplies.
2. **Flyback diodes** — on every inductive load, always.
3. **Galvanic isolation** — optocouplers or SSRs when switching mains.
4. **Watchdog-driven failsafe** — if firmware hangs, actuator returns to safe state.
5. **Physical kill switch** — mandatory on any build that can heat, cut, or move mass.

## Failure modes

- **Brownouts** from actuation surges — always a separate supply rail for motors; bulk cap on the MCU rail.
- **PWM interactions with radio** — high-current switching near the ESP32 antenna = dropped packets. Shield or physically separate.
- **Missed commands** — a retry-on-failure pattern on the software side often double-actuates; make Do idempotent where possible.

## Cross-references

- Personas: **DJ** (DMX), **locksmith** (solenoid), **HVAC tech** (relay), **parent** (white noise speaker), **theater stage manager** (cue dispatcher), **beekeeper** (ventilation fan).
- Flagship projects: [`projects/dj-bpm-sympath/`](../projects/dj-bpm-sympath/), [`projects/beekeeper-hivelink/`](../projects/beekeeper-hivelink/), [`projects/investigative-journalist-sourceshield/`](../projects/investigative-journalist-sourceshield/).
