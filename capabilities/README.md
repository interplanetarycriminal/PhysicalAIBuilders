# The Seven Base Capabilities

Every Physical-AI build can be decomposed into one or more of these verbs. Think of them as the parts-of-speech of the ESP32 world: you will almost never use just one.

| # | Verb | What it does | Typical ESP32 role |
|---|---|---|---|
| 1 | [**Generate**](generate.md) | Make new data from prompts or state | TTS, light patterns, audio synth, generative actuation |
| 2 | [**Transform**](transform.md) | Reshape data into a new representation | FFT, gesture classification, depth from stereo, ASR |
| 3 | [**Distill**](distill.md) | Compress many signals into one insight | "Risky window 14:00–17:00", HRV summary |
| 4 | [**Find**](find.md) | Locate patterns, anomalies, or objects | Anomaly detection, wake-word, face/person detect |
| 5 | [**Reason**](reason.md) | Infer causality, intent, or next action | On-device rule engine, off-device LLM |
| 6 | [**Do**](do.md) | Actuate the world | Motors, solenoids, IR, DMX, thermal printer |
| 7 | [**Represent**](represent.md) | Externalize state to humans or agents | OLED, e-paper, NeoPixel, haptic, sonification |

## How these chain together

The asymmetry loop in [`../framework/information-asymmetry.md`](../framework/information-asymmetry.md) is exactly these verbs strung together:

```
 sense(Find) → Transform → Distill → Represent
     ↑                                   ↓
    Do ←──── Reason ←── (human / agent reads Represent)
```

## How to use these docs

Each verb's file lists:
- **Definition & intent** in this repo's vocabulary.
- **Classic ESP32 implementations** (on-device and with companion software).
- **Sensors & add-ons** that participate in this verb (links into [`../hardware/`](../hardware/)).
- **Failure modes** — what goes wrong and how to notice.
- **Cross-references** to personas and flagship projects that lean heavily on this verb.
