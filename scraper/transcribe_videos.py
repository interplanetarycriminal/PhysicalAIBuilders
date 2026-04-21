#!/usr/bin/env python3
"""
transcribe_videos.py — fetch transcripts for every video in videos.json.

Strategy (cheap-first, always local):
  1. For each video, try YouTube's own subtitles via yt-dlp
     (`writesubtitles=True`, `writeautomaticsub=True`, `skip_download=True`).
     This is fast and free — ~90% of the catalog already has auto-captions.
  2. If none are available AND `--whisper` is passed, fall back to
     downloading audio-only (m4a/webm) and transcribing with
     `faster-whisper` locally. Models default to `small`. Heavier models
     are better; this is a knob.

Outputs
-------
  output/transcripts/<video_id>.vtt             # raw subs (cleanly named)
  output/transcripts/<video_id>.txt             # plain-text (cue-merged)
  output/transcripts/<video_id>.json            # chunked, timestamped, ready for LLM
  output/transcripts/INDEX.json                 # inventory (source, lang, whisper?)

Usage
-----
  # default — auto-captions only, skip anything that doesn't have them
  python transcribe_videos.py

  # also run Whisper locally for anything without captions (needs ffmpeg)
  python transcribe_videos.py --whisper --whisper-model small.en

  # scope — only videos whose title matches a regex
  python transcribe_videos.py --query 'esp32|csi|tinyml'
"""
from __future__ import annotations

import argparse
import json
import logging
import re
import subprocess
import sys
import time
from pathlib import Path
from typing import Any

import yt_dlp


SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_OUT = SCRIPT_DIR / "output"

log = logging.getLogger("physicalai.transcribe")


def load_videos(path: Path) -> list[dict[str, Any]]:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def vtt_to_plain(vtt_text: str) -> str:
    """Strip VTT cue headers / timestamps and de-duplicate rolling caption overlap."""
    lines: list[str] = []
    for raw in vtt_text.splitlines():
        s = raw.strip()
        if not s or s.startswith("WEBVTT") or "-->" in s or s.startswith("NOTE") or s.startswith("Kind:") or s.startswith("Language:"):
            continue
        # strip inline timing tags like <00:00:01.520><c>text</c>
        s = re.sub(r"<[^>]+>", "", s)
        if not s:
            continue
        if lines and lines[-1] == s:
            continue
        lines.append(s)
    # de-duplicate consecutive repeats across windows — YouTube auto-caps roll heavily
    deduped: list[str] = []
    for line in lines:
        if not deduped or not line.startswith(deduped[-1][-min(40, len(deduped[-1])):]):
            deduped.append(line)
    return "\n".join(deduped).strip()


def vtt_to_chunks(vtt_text: str, target_chars: int = 1200) -> list[dict[str, Any]]:
    """Produce [{start, end, text}] chunks suited to LLM summarization (~1200 char chunks)."""
    cues: list[tuple[float, float, str]] = []
    time_re = re.compile(r"(\d+):(\d+):(\d+[.,]\d+)\s*-->\s*(\d+):(\d+):(\d+[.,]\d+)")
    cur_start: float | None = None
    cur_end: float | None = None
    cur_lines: list[str] = []
    for raw in vtt_text.splitlines():
        m = time_re.search(raw)
        if m:
            if cur_start is not None and cur_lines:
                cues.append((cur_start, cur_end or cur_start, " ".join(cur_lines).strip()))
            def to_sec(h: str, mn: str, s: str) -> float:
                return int(h) * 3600 + int(mn) * 60 + float(s.replace(",", "."))
            cur_start = to_sec(m.group(1), m.group(2), m.group(3))
            cur_end = to_sec(m.group(4), m.group(5), m.group(6))
            cur_lines = []
        else:
            s = raw.strip()
            if not s or s.startswith("WEBVTT") or s.startswith("NOTE"):
                continue
            s = re.sub(r"<[^>]+>", "", s)
            if s:
                cur_lines.append(s)
    if cur_start is not None and cur_lines:
        cues.append((cur_start, cur_end or cur_start, " ".join(cur_lines).strip()))

    # merge cues into ~target_chars chunks
    chunks: list[dict[str, Any]] = []
    buf_text = ""
    buf_start: float | None = None
    buf_end: float = 0.0
    for start, end, text in cues:
        if buf_start is None:
            buf_start = start
        buf_text = (buf_text + " " + text).strip() if buf_text else text
        buf_end = end
        if len(buf_text) >= target_chars:
            chunks.append({"start": buf_start, "end": buf_end, "text": buf_text})
            buf_text = ""
            buf_start = None
    if buf_text and buf_start is not None:
        chunks.append({"start": buf_start, "end": buf_end, "text": buf_text})
    return chunks


def fetch_youtube_subs(url: str, tmp_dir: Path, langs: list[str]) -> Path | None:
    """Attempt to get YouTube-provided subs (manual first, auto fallback). Returns .vtt path or None."""
    opts = {
        "quiet": True,
        "no_warnings": True,
        "skip_download": True,
        "writesubtitles": True,
        "writeautomaticsub": True,
        "subtitleslangs": langs,
        "subtitlesformat": "vtt",
        "outtmpl": str(tmp_dir / "%(id)s.%(ext)s"),
        "ignoreerrors": True,
    }
    try:
        with yt_dlp.YoutubeDL(opts) as ydl:
            ydl.extract_info(url, download=True)
    except Exception as exc:  # noqa: BLE001
        log.warning("subs fetch failed for %s: %s", url, exc)
        return None
    # find any .vtt that yt-dlp wrote for this video
    for lang in langs + ["en", "en-US", "en-GB"]:
        for p in tmp_dir.glob(f"*.{lang}.vtt"):
            return p
    # any .vtt at all
    for p in tmp_dir.glob("*.vtt"):
        return p
    return None


def fetch_audio(url: str, tmp_dir: Path) -> Path | None:
    opts = {
        "quiet": True,
        "no_warnings": True,
        "format": "bestaudio/best",
        "outtmpl": str(tmp_dir / "%(id)s.%(ext)s"),
        "ignoreerrors": True,
    }
    try:
        with yt_dlp.YoutubeDL(opts) as ydl:
            info = ydl.extract_info(url, download=True)
    except Exception as exc:  # noqa: BLE001
        log.warning("audio fetch failed for %s: %s", url, exc)
        return None
    if not info:
        return None
    vid = info.get("id", "")
    for p in tmp_dir.glob(f"{vid}.*"):
        if p.suffix in {".m4a", ".webm", ".mp3", ".opus", ".mp4"}:
            return p
    return None


def whisper_transcribe(audio: Path, model_name: str) -> tuple[str, list[dict[str, Any]]] | None:
    try:
        from faster_whisper import WhisperModel
    except ImportError:
        log.error("faster-whisper not installed. `pip install faster-whisper` (and ffmpeg on PATH).")
        return None
    log.info("whisper: transcribing %s with model=%s", audio.name, model_name)
    model = WhisperModel(model_name, device="auto", compute_type="auto")
    segments, info = model.transcribe(str(audio), vad_filter=True)
    full_lines: list[str] = []
    chunks: list[dict[str, Any]] = []
    buf_text = ""
    buf_start: float | None = None
    buf_end: float = 0.0
    target_chars = 1200
    for seg in segments:
        full_lines.append(seg.text.strip())
        if buf_start is None:
            buf_start = seg.start
        buf_text = (buf_text + " " + seg.text).strip()
        buf_end = seg.end
        if len(buf_text) >= target_chars:
            chunks.append({"start": buf_start, "end": buf_end, "text": buf_text})
            buf_text = ""
            buf_start = None
    if buf_text and buf_start is not None:
        chunks.append({"start": buf_start, "end": buf_end, "text": buf_text})
    plain = " ".join(full_lines).strip()
    return plain, chunks


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Fetch transcripts for every video in videos.json.")
    p.add_argument("--videos-json", type=Path, default=DEFAULT_OUT / "videos.json")
    p.add_argument("--out", type=Path, default=DEFAULT_OUT / "transcripts")
    p.add_argument("--query", type=str, default=None, help="Only transcribe videos whose title matches this regex.")
    p.add_argument("--langs", type=str, default="en,en-US,en-GB")
    p.add_argument("--whisper", action="store_true", help="Fallback to local Whisper for videos without subs.")
    p.add_argument("--whisper-model", type=str, default="small.en",
                   help="faster-whisper model size: tiny.en, base.en, small.en, medium.en, large-v3.")
    p.add_argument("--sleep", type=float, default=1.0)
    p.add_argument("--max", type=int, default=None, help="Max videos to transcribe this run.")
    p.add_argument("--verbose", action="store_true")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO,
                        format="%(asctime)s %(levelname)s %(message)s")

    args.out.mkdir(parents=True, exist_ok=True)
    tmp_dir = args.out / "_tmp"
    tmp_dir.mkdir(parents=True, exist_ok=True)

    videos = load_videos(args.videos_json)
    query_re = re.compile(args.query, re.IGNORECASE) if args.query else None
    if query_re:
        videos = [v for v in videos if query_re.search(v.get("title", ""))]
    if args.max:
        videos = videos[: args.max]

    langs = [l.strip() for l in args.langs.split(",") if l.strip()]
    index_path = args.out / "INDEX.json"
    inventory: dict[str, dict[str, Any]] = {}
    if index_path.exists():
        try:
            inventory = json.loads(index_path.read_text(encoding="utf-8"))
        except Exception:  # noqa: BLE001
            inventory = {}

    for i, v in enumerate(videos):
        vid = v.get("video_id") or ""
        if not vid:
            continue
        if vid in inventory and inventory[vid].get("status") == "ok":
            log.debug("[%d/%d] already have transcript for %s, skipping", i + 1, len(videos), vid)
            continue
        url = v.get("url", f"https://www.youtube.com/watch?v={vid}")
        title = v.get("title", "")
        log.info("[%d/%d] %s — %s", i + 1, len(videos), vid, title[:80])

        # stage 1: YouTube-provided subs
        sub_path = fetch_youtube_subs(url, tmp_dir, langs)
        source = None
        vtt_text = None
        if sub_path and sub_path.exists():
            vtt_text = sub_path.read_text(encoding="utf-8", errors="replace")
            source = "youtube_subs"

        plain_text = None
        chunks: list[dict[str, Any]] = []

        if vtt_text:
            (args.out / f"{vid}.vtt").write_text(vtt_text, encoding="utf-8")
            plain_text = vtt_to_plain(vtt_text)
            chunks = vtt_to_chunks(vtt_text)
        elif args.whisper:
            audio = fetch_audio(url, tmp_dir)
            if audio and audio.exists():
                result = whisper_transcribe(audio, args.whisper_model)
                if result:
                    plain_text, chunks = result
                    source = f"whisper:{args.whisper_model}"
                try:
                    audio.unlink()
                except OSError:
                    pass

        if plain_text:
            (args.out / f"{vid}.txt").write_text(plain_text, encoding="utf-8")
            (args.out / f"{vid}.json").write_text(json.dumps({
                "video_id": vid,
                "title": title,
                "url": url,
                "creator_handle": v.get("creator_handle"),
                "source": source,
                "chunks": chunks,
            }, ensure_ascii=False, indent=2), encoding="utf-8")
            inventory[vid] = {"status": "ok", "source": source, "chars": len(plain_text)}
        else:
            inventory[vid] = {"status": "no_transcript", "source": None}
            log.info("  no captions, whisper disabled — skipped")

        index_path.write_text(json.dumps(inventory, indent=2), encoding="utf-8")
        if args.sleep > 0:
            time.sleep(args.sleep)

    # clean _tmp
    for p in tmp_dir.glob("*"):
        try:
            p.unlink()
        except OSError:
            pass
    try:
        tmp_dir.rmdir()
    except OSError:
        pass

    ok = sum(1 for v in inventory.values() if v.get("status") == "ok")
    log.info("done — %d / %d videos transcribed", ok, len(inventory))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
