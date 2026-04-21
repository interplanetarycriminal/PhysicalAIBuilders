# Cold-Case Sleuth — TimeFold

> **Level:** 100 — Research-grade. Semester+. $$$$ (many nodes, a real agent, real ethics review).
> **Tribe:** Strange & archetypal / Knowledge workers (#83)
> **One-line vibe:** a distributed, retention-minded Physical-AI network that finds *pattern breaks in time* across a space — and turns them into investigative leads for a patient human sleuth.

## The asymmetry

Most home sensor deployments answer "what's happening now?" TimeFold answers "what has been **unlike the usual** over the last 30, 90, 365 days — across many places at once?" It is a memory system disguised as a sensor network. Paired with a Claude / local LLM agent trained to think like a detective, it surfaces leads no human would have patience for.

This is the top of the ladder because it combines *every* other capability:

- **Find** (anomaly detection).
- **Transform** (features from many modalities into a time-aligned embedding space).
- **Distill** (a day compressed into a vector).
- **Reason** (the agent that reads deviations and hypothesizes).
- **Represent** (a visual timeline, an e-paper daily card, a queryable chat surface).
- **Generate** (the agent drafts investigative hypotheses).
- **Do** (the agent's recommendations can fire notifications, trigger cameras to cache).

## The two-line spec

```
HARDWARE:  mesh of ESP32-S3 nodes, each (BLE + mmWave + mic RMS + BME280 + optional AS7341),
           a Raspberry Pi 5 / mini PC gateway, a time-series DB (TimescaleDB / InfluxDB),
           optional Coral TPU / local GPU for embeddings.
SOFTWARE:  node firmware with tool-surface + ingest pipeline + vector-DB of daily embeddings
           + Claude agent with investigative tools + reviewer UI.
```

## Bill of materials (per site — 6-node home/office deployment)

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 6 | XIAO ESP32-S3 Sense | Node MCU + mic | Seeed | $18 each |
| 6 | LD2410B | mmWave presence | AliExpress | $5 each |
| 6 | BME280 | ambient | Adafruit | $10 each |
| 2 | AS7341 (on gateway sites) | spectral | SparkFun | $17 each |
| 1 | Raspberry Pi 5 8GB | Gateway + DB | Pi shop | $80 |
| 1 | Coral USB Accelerator | embedding pipeline | coral.ai | $60 |
| 1 | 1 TB SSD | retention | any | $80 |
| 6 | 18650 + enclosure / USB-PD | Power | — | $20 each |

**Total:** ~$600 at the low end; scales with nodes and retention.

## Design

### Time-folding

Each node publishes a 1 min rollup every minute. The gateway:
1. Aligns rollups to UTC minute buckets.
2. Every 60 min, computes a 128-dim embedding of that hour's multi-modal state (per-node + cross-node features).
3. Every day at 00:30 local, computes a *daily* embedding.
4. Anomaly detection lives in embedding space — "how unusual is today vs the last 90 days at this site?"

### The investigator agent

A Claude (or Ollama) agent is given tools:

- `list_anomalies(since, site)` → top-N deviations by embedding distance.
- `playback(anomaly_id, window_min)` → returns the minute-level rollups before/during/after.
- `cross_site_compare(anomaly_id)` → returns concurrent state at other sites.
- `annotate(anomaly_id, note)` → lets the human add context.
- `check_calendar(window)`, `check_weather(window)` → external signals.

System prompt: *"You are a patient investigator. Given anomalies in a sensor history, propose the most boring explanation first, rule it out with evidence, then escalate. Always show your work; cite the specific tool results."*

### The reviewer UI

A small web app:
- Timeline with unusual days highlighted.
- Click a day → see the agent's hypothesis + the tool traces.
- Accept / reject / annotate.
- Resolved anomalies go into a labeled training set.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp) for the node-side tool surface. Shared with the Level-80 ghost-hunter project in structure, extended with a per-node **embedding seed** computed on-device.

## Companion software

See [`companion/README.md`](companion/README.md) for the full agent + ingest pipeline.

## Ethics — the real Level 100 work

- **Subjects of sensing must be informed.** This is sharper here than in any other project in the repo.
- **Retention is a design choice.** Default: raw-minute data expires in 48 hours; daily embeddings persist 365 days. Expose this knob, don't bury it.
- **Agent-initiated action requires dual-control.** The agent proposes; a human commits. Do not wire the agent to alert a third party without a human in the loop.
- **Dataset contribution requires consent + anonymization.** Any aggregate you share with the community goes through an opt-in + k-anonymity pass.
- **Think about the bad versions.** Who would weaponize this? Design in countermeasures (audibility, visible indicators, local-only mode, kill switch).

## Research-grade deliverables (what "Level 100" means)

- **A deployment write-up** with calibration procedures, baseline characterization, and limitations.
- **A reproducibility kit** — BOM, firmware, Docker-composed gateway, sample data.
- **A paper draft** — novel claim about some fusion or pattern you couldn't see before. Partner with a university group if possible.
- **Ethics review** — walk your institution / group's ethics board through the design.

## Extension paths beyond Level 100

- Research-grade cross-household datasets (with formal ethics approval).
- Contribution to public sensor standards (schema, vocabulary, retention defaults).
- A real-world longitudinal study: what *do* houses actually reveal across a year?

## Credits & inspiration

Ambient-intelligence research at MIT Media Lab, the Smart Buildings research community, and every patient investigator in fiction who kept looking at the evidence after everyone else went home.
