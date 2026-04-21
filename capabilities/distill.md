# Distill

> Compress many signals into one salient, decision-ready insight.

## In this repo's vocabulary

Distill is the *collapse* verb. A day of air-quality readings becomes "cook a meal before 14:00 or the kids will smell it all evening". A week of sleep-sensor logs becomes "your REM crashed on the two nights you drank past 9pm."

Distill is what makes a data logger a *witness*.

## Classic ESP32 implementations

### On-device
- **Rolling summary statistics** — means, percentiles, Z-scores over windows.
- **Event extraction** — threshold crossings, peak detection (MAX30102 SpO2 event).
- **Rule-engine distills** — "when CO2 > 1200 for 10 min → alert"; implemented with Node-RED-style flow on-device using MicroPython state machines.
- **Simple model summaries** — a k-means cluster id for the day's behavior, stored as a single byte.

### With companion software
- **LLM narrator** — ESP32 sends a window of rolling stats → LLM returns 1-sentence narrative: *"You held your breath three times while debugging."*
- **Time-series DB + alerts** — InfluxDB + Grafana alerts, narrated to email/Slack/push via a bot.
- **Digests** — a nightly agent writes an e-paper "tomorrow's forecast" card.

## Sensors & add-ons that feed Distill

- **Multi-sensor stacks** — BME680 + SEN55 + SCD41 distilled into one IAQ verdict.
- **Bio stacks** — HRV + temp + GSR distilled into one stress-index number.
- **RF scans** — BLE sniff history distilled into "who are your spatial neighbors this week".
- **Add-ons**: e-paper (perfect distill target — one display, one sentence), single NeoPixel ring as status.

## Failure modes

- **Over-distilling** — collapsing a thing so hard you erase the reason it mattered. Always keep a "why" column next to the summary.
- **Goodhart** — the moment you start optimizing for the distilled score, people fake the inputs.
- **Silent drift** — distilled outputs look fine while the sensor underneath has drifted. Always include a calibration-health indicator alongside the score.

## Cross-references

- Personas: **parent-sleep-ally**, **wildfire lookout**, **beekeeper**, **ER charge nurse**, **therapist**.
- Flagship projects: [`projects/parent-sleep-ally/`](../projects/parent-sleep-ally/), [`projects/homebrewer-fermentdoc/`](../projects/homebrewer-fermentdoc/), [`projects/wildfire-lookout-pyrosentinel/`](../projects/wildfire-lookout-pyrosentinel/).
