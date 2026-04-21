# Physical-AI YouTube Creator Scraper

A headless Python app that walks a curated list of YouTube creators who specialize in **Physical AI** and **ESP32** builds, and produces a clean index of every video they've published.

No GUI. No browser. No API key required. Uses [`yt-dlp`](https://github.com/yt-dlp/yt-dlp) in `--flat-playlist` mode so it pulls metadata (title, URL, upload date, duration) without downloading any video bytes.

## Why

The Physical-AI / ESP32 niche on YouTube is where the community's tacit knowledge lives — wiring mistakes, which sensor actually works, what the serial output really looks like. A searchable index of every video from the top creators is the single best learning asset you can build for this repo.

## What it does

1. Reads [`creators.yaml`](creators.yaml) — a curated list of handles / channel IDs, each tagged with a note on what they specialize in.
2. For each creator, asks `yt-dlp` for the flat playlist of their `/videos` tab.
3. Writes:
   - `output/videos.json` — one record per video across all creators (stable schema).
   - `output/videos.csv` — the same, flat.
   - `output/by-creator/<handle>.md` — human-readable digest.
   - `output/INDEX.md` — a top-level index linked from this README.
4. Optional `--query` flag filters to titles matching a regex (e.g. `--query 'esp32|esp-idf|tinyml|csi'`).

## Install

```bash
cd scraper
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
```

## Run

```bash
# Full pull
python fetch_physical_ai_videos.py

# Only titles matching a regex
python fetch_physical_ai_videos.py --query 'esp32|csi|tinyml|mmwave|lora|matter'

# Limit to N most-recent videos per creator (fast refresh)
python fetch_physical_ai_videos.py --limit 25

# Add or remove creators by editing creators.yaml, then rerun
```

## Output shape (`videos.json`)

```json
[
  {
    "creator_handle": "@AndreasSpiess",
    "creator_note": "Swiss maker, ESP32/ESP8266 deep dives, measurement-first.",
    "video_id": "xxxxxxxxxxx",
    "title": "...",
    "url": "https://www.youtube.com/watch?v=xxxxxxxxxxx",
    "upload_date": "2024-09-17",
    "duration_sec": 842,
    "scraped_at": "2026-04-21T12:34:56Z"
  }
]
```

## Ethics & etiquette

- We read **public** channel listings only; we do not download videos.
- Honor YouTube's robots.txt and rate limits — the default script sleeps between creators.
- This is a personal research tool. Do not host the generated index publicly without the creators' blessing; credit them.

## Extending

- Add creators in `creators.yaml`.
- Add or share unlisted community playlists in `seed_playlists.yaml` to grow the creator pool automatically.

## The full three-stage pipeline

This directory ships three composable scripts. Use them in order:

### Stage 1 — index every video

```bash
python fetch_physical_ai_videos.py
```

Output: `output/videos.json`, `output/INDEX.md`, `output/by-creator/*.md`.

### Stage 2 — fetch transcripts

Most videos already have YouTube auto-captions; those are free and fast. For ones that don't, fall back to local Whisper (you'll need `ffmpeg` on PATH and `faster-whisper` installed).

```bash
# Free / fast — YouTube subs only
python transcribe_videos.py

# Plus Whisper for anything without subs
python transcribe_videos.py --whisper --whisper-model small.en

# Only transcribe ESP32 / Physical-AI adjacent titles
python transcribe_videos.py --query 'esp32|csi|tinyml|sensor|matter|lora|mmwave'
```

Output: `output/transcripts/<video_id>.vtt`, `.txt`, and chunked `.json` ready for LLMs.

### Stage 3 — synthesize learning docs from transcripts

This is where the magic happens: we turn raw transcripts into tight, builder-grade learning documents — "what you learn", "hardware named", "software / techniques", "wiring & pitfalls", "riff on this" — by asking Claude (with prompt caching) to summarize each transcript.

```bash
export ANTHROPIC_API_KEY=sk-ant-...
python synthesize_docs.py --max 20           # start small, verify quality
python synthesize_docs.py                    # full corpus
```

Output: `output/learning_docs/<video_id>.md`, `INDEX.md`, and `_corpus_digest.md` (every "hardware named" section stitched together for cross-video search).

**Why this is useful.** Instead of watching 400 videos, you read 400 one-pagers that agree on structure — and you can grep across all of them for "BME680", "mmWave", "ESP-NOW", "ICE-40 FPGA beside an ESP32", etc. The `_corpus_digest.md` becomes the repository's tacit knowledge in a single searchable file.

## Seed playlists

Community members can share unlisted or public playlists that represent the tribe. Drop the URL into `seed_playlists.yaml` with a note; the next `fetch_physical_ai_videos.py` run walks the playlist, extracts the unique uploaders, and expands them into the creator pool automatically. De-duplication against `creators.yaml` is handled for you.
