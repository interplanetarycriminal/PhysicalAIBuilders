#!/usr/bin/env python3
"""
synthesize_docs.py — turn transcripts into learning documentation.

Reads the chunked JSON transcripts produced by transcribe_videos.py and
asks Claude (Anthropic SDK) to produce, per video, a single Markdown
learning-doc with:

  - What the viewer learns (bulleted takeaways)
  - Hardware named (ESP32 variant, sensors, modules)
  - Software techniques (libraries, algorithms, TinyML, LLMs)
  - Wiring / pitfalls worth copying into hardware/ docs
  - A "riff on this" — how to extend into a PhysicalAIBuilders project,
    referencing the seven base capabilities in ../capabilities/

Then produces a corpus-level digest that pulls every "hardware named"
entry into a single appendix for quick search.

Prompt-caching is used so repeated summarizations are cheap — we cache
the long system prompt (the learning-doc spec) and vary only the
per-video chunks.

Usage
-----
  # one-time
  export ANTHROPIC_API_KEY=sk-ant-...
  python synthesize_docs.py                # synth every transcript
  python synthesize_docs.py --max 10       # try on 10 first
  python synthesize_docs.py --model claude-sonnet-4-6

Outputs
-------
  output/learning_docs/<video_id>.md
  output/learning_docs/INDEX.md
  output/learning_docs/_corpus_digest.md
"""
from __future__ import annotations

import argparse
import json
import logging
import os
import sys
import time
from pathlib import Path
from typing import Any


SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_TRANSCRIPTS = SCRIPT_DIR / "output" / "transcripts"
DEFAULT_OUT = SCRIPT_DIR / "output" / "learning_docs"
DEFAULT_MODEL = "claude-sonnet-4-6"

log = logging.getLogger("physicalai.synthesize")


SYSTEM_PROMPT = """You are a Physical-AI Builders archivist.

You are reading a transcript of a YouTube video by a creator who specializes
in ESP32 microcontrollers, embedded ML ("TinyML"), sensors, and Physical AI
builds. Your job is to turn this transcript into a tight learning document
a builder can read in two minutes and know EXACTLY what the video taught.

OUTPUT FORMAT (strict — Markdown):

```
# {video title}

**Creator:** {handle}
**URL:** {url}
**Source:** {transcript source: youtube_subs or whisper:...}

## What you learn
- 3–8 short bullets. Each bullet is ONE concrete takeaway.

## Hardware named
- List every specific part (ESP32 variant, sensor, breakout, display,
  actuator, module) that appears in the video. For each: name, one-line
  note on what it does here.

## Software / techniques
- Libraries, SDKs, algorithms, models, protocols actually used.

## Wiring & pitfalls
- Any pin-level gotcha, power supply note, timing quirk, or failure mode
  the creator ran into. Be terse — one line each.

## Riff on this
- 2–3 bullets on how a PhysicalAIBuilders community member could extend
  this build. Reference relevant base capabilities from
  Generate / Transform / Distill / Find / Reason / Do / Represent.
- Name the information asymmetry it creates (what does the wielder know
  that others don't?).

## Confidence
- One word: high | medium | low — how confident are you in the doc given
  transcript quality. If low, say why in a half-sentence.
```

HARD RULES:
- If the transcript is clearly off-topic (not ESP32 / Physical AI / sensors /
  embedded ML), output ONLY: `_off-topic: <one-line reason>_` and nothing else.
- Do not invent parts not mentioned. If a part is ambiguous, say "likely X".
- Keep the whole document under 500 words.
- Never include filler like "in this video...". Get to the signal.
"""


def load_chunks(path: Path) -> dict[str, Any]:
    return json.loads(path.read_text(encoding="utf-8"))


def build_user_message(doc: dict[str, Any]) -> str:
    header = (
        f"Video: {doc.get('title', '')}\n"
        f"Creator: {doc.get('creator_handle', '')}\n"
        f"URL: {doc.get('url', '')}\n"
        f"Transcript source: {doc.get('source', 'unknown')}\n\n"
        "Transcript chunks (with timestamps in seconds):\n\n"
    )
    body_parts: list[str] = []
    for c in doc.get("chunks", []):
        body_parts.append(f"[{c.get('start', 0):.0f}–{c.get('end', 0):.0f}s]\n{c.get('text', '')}\n")
    return header + "\n".join(body_parts)


def call_claude(client, model: str, system_prompt: str, user_msg: str) -> str:
    # Prompt caching on the system block so re-synthesis is cheap.
    resp = client.messages.create(
        model=model,
        max_tokens=1400,
        system=[
            {
                "type": "text",
                "text": system_prompt,
                "cache_control": {"type": "ephemeral"},
            }
        ],
        messages=[{"role": "user", "content": user_msg}],
    )
    return "".join(block.text for block in resp.content if getattr(block, "type", "") == "text")


def main() -> int:
    p = argparse.ArgumentParser(description="Turn transcripts into learning docs via Claude.")
    p.add_argument("--transcripts", type=Path, default=DEFAULT_TRANSCRIPTS)
    p.add_argument("--out", type=Path, default=DEFAULT_OUT)
    p.add_argument("--model", type=str, default=DEFAULT_MODEL)
    p.add_argument("--max", type=int, default=None)
    p.add_argument("--sleep", type=float, default=0.5)
    p.add_argument("--force", action="store_true", help="Redo even if learning doc exists.")
    args = p.parse_args()

    logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s %(message)s")

    try:
        import anthropic  # type: ignore
    except ImportError:
        log.error("anthropic SDK not installed. `pip install anthropic`.")
        return 2

    api_key = os.environ.get("ANTHROPIC_API_KEY")
    if not api_key:
        log.error("ANTHROPIC_API_KEY not set.")
        return 2
    client = anthropic.Anthropic(api_key=api_key)

    args.out.mkdir(parents=True, exist_ok=True)
    json_files = sorted(args.transcripts.glob("*.json"))
    # Exclude the INDEX.json inventory
    json_files = [p for p in json_files if p.name != "INDEX.json"]
    if args.max:
        json_files = json_files[: args.max]

    produced: list[tuple[str, str]] = []
    for i, jf in enumerate(json_files):
        vid = jf.stem
        out_md = args.out / f"{vid}.md"
        if out_md.exists() and not args.force:
            log.debug("skip %s — exists", vid)
            produced.append((vid, out_md.read_text(encoding="utf-8").splitlines()[0] if out_md.exists() else vid))
            continue
        doc = load_chunks(jf)
        if not doc.get("chunks"):
            log.info("skip %s — no chunks", vid)
            continue
        user_msg = build_user_message(doc)
        try:
            md = call_claude(client, args.model, SYSTEM_PROMPT, user_msg)
        except Exception as exc:  # noqa: BLE001
            log.warning("Claude call failed for %s: %s", vid, exc)
            continue
        out_md.write_text(md.strip() + "\n", encoding="utf-8")
        first_line = md.strip().splitlines()[0] if md.strip() else vid
        produced.append((vid, first_line))
        log.info("[%d/%d] wrote %s", i + 1, len(json_files), out_md.name)
        time.sleep(args.sleep)

    # index
    index = ["# Learning docs index\n"]
    for vid, first_line in sorted(produced):
        index.append(f"- [{first_line.lstrip('# ').strip()}]({vid}.md)")
    (args.out / "INDEX.md").write_text("\n".join(index) + "\n", encoding="utf-8")

    # corpus digest: concatenate "Hardware named" sections from every doc
    digest = ["# Corpus digest — hardware named across all videos\n",
              "_Auto-assembled from per-video learning docs._\n"]
    for md_path in sorted(args.out.glob("*.md")):
        if md_path.name in {"INDEX.md", "_corpus_digest.md"}:
            continue
        text = md_path.read_text(encoding="utf-8")
        if "## Hardware named" not in text:
            continue
        block = text.split("## Hardware named", 1)[1]
        block = block.split("\n## ", 1)[0]
        first_line = text.splitlines()[0].lstrip("# ").strip()
        digest.append(f"\n### {first_line} (`{md_path.stem}`)\n{block.strip()}\n")
    (args.out / "_corpus_digest.md").write_text("\n".join(digest), encoding="utf-8")

    log.info("done — %d learning docs in %s", len(produced), args.out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
