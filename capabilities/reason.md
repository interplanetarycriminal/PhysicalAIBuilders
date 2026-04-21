# Reason

> Infer causality, intent, or the best next action across modalities.

## In this repo's vocabulary

Reason is the *because* verb. Find tells you *that* there's a presence behind the door; Reason tells you *why* (delivery person at 14:02, matches your package-tracker; the dog doesn't react because it's a known scent). Reason is where the software twin in [`../framework/software-hardware-pairing.md`](../framework/software-hardware-pairing.md) usually lives.

## Classic ESP32 implementations

### On-device
- **Rule engines / state machines** — explicit if-this-then-that that codifies knowledge.
- **Bayesian nets** — small enough to run on ESP32-S3 for simple causal inference.
- **Tiny decision trees / random forests** — Edge Impulse deploys these readily.
- **Reinforcement-learned policies** — table-Q or tiny MLP; ESP32-S3 can run both for simple actuation.

### With companion software
- **LLM-as-reasoner** — ESP32 exposes tools (read sensor, set actuator). An LLM (Claude, GPT-4-class, or local 7–13B) orchestrates.
  - Build pattern: ESP32 exposes an MCP-ish JSON-RPC surface; the LLM calls `read_scene()`, `look_up_history(minutes=60)`, `narrate(tone='dry')`, `act(motor='fan', pwm=40)`.
- **Multi-modal vision-language models** — feed a camera frame + mic transcript + IMU window into a VLM, get a situation report.
- **Causal inference services** — off-device Bayesian / do-calculus engines for "did the HVAC cause the sleep drop?"

## Sensors & add-ons that feed Reason

Reason consumes the *outputs* of Find, Transform, Distill. It rarely touches a sensor directly; it reads the already-structured evidence and fuses it with:

- Historical context (on-device SD + cloud DB).
- External knowledge (LLM world knowledge, calendars, weather APIs).
- User intent (spoken, buttoned, app-selected).

## Failure modes

- **LLM hallucination in the loop** — ground every LLM-initiated action in a hard on-device guard: "LLM can request 'fan up' but firmware rejects if ambient temp is fine."
- **Brittle rules** — rule engines rot fast; budget for review.
- **Latency** — if Reason is the bottleneck, the system feels slow; keep reactive actions on-device, deliberative actions off-device.

## Cross-references

- Personas: **investigative journalist**, **therapist**, **new manager**, **forensic accountant**, **ER charge nurse**, **cold-case sleuth**.
- Flagship projects: [`projects/cold-case-sleuth-timefold/`](../projects/cold-case-sleuth-timefold/), [`projects/parent-sleep-ally/`](../projects/parent-sleep-ally/), [`projects/ar-garden-oracle/`](../projects/ar-garden-oracle/).
