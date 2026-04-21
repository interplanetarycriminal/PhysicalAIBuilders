#!/usr/bin/env python3
"""
Headless scraper: walk a curated list of Physical-AI / ESP32 YouTube creators
and produce a clean index of every video (title, URL, date, duration).

Uses yt-dlp's --flat-playlist (via the Python API) so we fetch metadata only,
never video bytes. No API key required. No browser. No GUI.

Usage
-----
    python fetch_physical_ai_videos.py
    python fetch_physical_ai_videos.py --query 'esp32|csi|tinyml|mmwave|lora'
    python fetch_physical_ai_videos.py --limit 25
    python fetch_physical_ai_videos.py --creators-file creators.yaml --out output/

Outputs
-------
    output/videos.json                # one record per video, all creators
    output/videos.csv                 # same, flat CSV
    output/by-creator/<handle>.md     # human-readable digest per creator
    output/INDEX.md                   # top-level index, links into by-creator/
    output/errors.log                 # per-creator resolution failures
"""
from __future__ import annotations

import argparse
import csv
import datetime as dt
import json
import logging
import os
import re
import sys
import time
from pathlib import Path
from typing import Any

import yaml
import yt_dlp


SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_CREATORS = SCRIPT_DIR / "creators.yaml"
DEFAULT_PLAYLISTS = SCRIPT_DIR / "seed_playlists.yaml"
DEFAULT_OUT = SCRIPT_DIR / "output"

log = logging.getLogger("physicalai.scraper")


def load_creators(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}
    entries: list[dict[str, str]] = []
    for c in data.get("creators", []) or []:
        handle = (c.get("handle") or "").strip()
        if not handle:
            continue
        entries.append({"identifier": handle, "note": c.get("note", "")})
    for cid in data.get("channel_ids", []) or []:
        cid = (cid or "").strip()
        if not cid:
            continue
        entries.append({"identifier": cid, "note": ""})
    return entries


def load_seed_playlists(path: Path) -> list[dict[str, str]]:
    if not path.exists():
        return []
    with path.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}
    playlists: list[dict[str, str]] = []
    for p in data.get("playlists", []) or []:
        url = (p.get("url") or "").strip()
        if not url:
            continue
        playlists.append({"url": url, "note": p.get("note", "")})
    return playlists


def expand_seed_playlist(url: str) -> list[dict[str, str]]:
    """Walk a playlist and return unique uploaders we discover.

    Returns a list of {identifier, note} dicts compatible with load_creators output.
    Uses `extract_flat=True` so we get each entry's uploader without loading full pages.
    """
    ydl_opts = {
        "quiet": True,
        "no_warnings": True,
        "skip_download": True,
        "extract_flat": True,  # full flat — includes uploader info per entry
        "ignoreerrors": True,
    }
    uploaders: dict[str, str] = {}
    try:
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            info = ydl.extract_info(url, download=False)
    except Exception as exc:  # noqa: BLE001
        log.warning("seed playlist %s failed: %s", url, exc)
        return []
    for e in (info or {}).get("entries") or []:
        if not e:
            continue
        handle = e.get("uploader_id") or ""
        uploader = e.get("uploader") or ""
        # Prefer an @handle if available; otherwise fall back to channel URL/ID.
        channel_id = e.get("channel_id")
        ident = handle if handle.startswith("@") else (channel_id or handle or "")
        if not ident:
            continue
        uploaders.setdefault(ident, uploader)
    return [{"identifier": ident, "note": f"Discovered via seed playlist (uploader: {name})"} for ident, name in uploaders.items()]


def channel_videos_url(identifier: str) -> str:
    """Resolve a handle (@Name) or channel_id (UC...) to the /videos tab URL."""
    ident = identifier.strip()
    if ident.startswith("@"):
        return f"https://www.youtube.com/{ident}/videos"
    if ident.startswith("UC") and len(ident) >= 20:
        return f"https://www.youtube.com/channel/{ident}/videos"
    # fall back to a search-style URL the user probably meant
    return f"https://www.youtube.com/{ident}/videos"


def fetch_one(identifier: str, limit: int | None) -> list[dict[str, Any]]:
    """Return flat video metadata for a single creator. Never raises; logs on failure."""
    url = channel_videos_url(identifier)
    ydl_opts = {
        "quiet": True,
        "no_warnings": True,
        "skip_download": True,
        "extract_flat": "in_playlist",
        "ignoreerrors": True,
        "playlist_items": f"1-{limit}" if limit else None,
    }
    try:
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            info = ydl.extract_info(url, download=False)
    except Exception as exc:  # noqa: BLE001 — we never want to crash the run
        log.warning("resolution failed for %s: %s", identifier, exc)
        return []

    entries: list[dict[str, Any]] = []
    raw_entries = (info or {}).get("entries") or []
    for e in raw_entries:
        if not e:
            continue
        video_id = e.get("id") or ""
        title = e.get("title") or ""
        duration = e.get("duration")
        upload_ts = e.get("timestamp")
        upload_date = None
        if upload_ts:
            upload_date = dt.datetime.fromtimestamp(upload_ts, dt.timezone.utc).date().isoformat()
        entries.append(
            {
                "video_id": video_id,
                "title": title,
                "url": f"https://www.youtube.com/watch?v={video_id}" if video_id else e.get("url", ""),
                "upload_date": upload_date,
                "duration_sec": int(duration) if isinstance(duration, (int, float)) else None,
            }
        )
    return entries


def matches(query_re: re.Pattern[str] | None, title: str) -> bool:
    return bool(query_re.search(title)) if query_re else True


def write_outputs(records: list[dict[str, Any]], out_dir: Path, creators: list[dict[str, str]]) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "by-creator").mkdir(parents=True, exist_ok=True)

    # videos.json
    with (out_dir / "videos.json").open("w", encoding="utf-8") as f:
        json.dump(records, f, ensure_ascii=False, indent=2)

    # videos.csv
    fields = ["creator_handle", "creator_note", "video_id", "title", "url", "upload_date", "duration_sec", "scraped_at"]
    with (out_dir / "videos.csv").open("w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in records:
            w.writerow({k: r.get(k, "") for k in fields})

    # per-creator markdown
    by_creator: dict[str, list[dict[str, Any]]] = {}
    for r in records:
        by_creator.setdefault(r["creator_handle"], []).append(r)

    for handle, rows in sorted(by_creator.items()):
        safe = re.sub(r"[^A-Za-z0-9_.@-]", "_", handle)
        with (out_dir / "by-creator" / f"{safe}.md").open("w", encoding="utf-8") as f:
            note = rows[0].get("creator_note", "")
            f.write(f"# {handle}\n\n")
            if note:
                f.write(f"> {note}\n\n")
            f.write(f"Total videos indexed: **{len(rows)}**\n\n")
            f.write("| Date | Duration | Title |\n|---|---|---|\n")
            for r in sorted(rows, key=lambda x: x.get("upload_date") or "", reverse=True):
                dur = r.get("duration_sec")
                dur_str = f"{dur // 60}m{dur % 60:02d}s" if isinstance(dur, int) else ""
                title = (r.get("title") or "").replace("|", "\\|")
                f.write(f"| {r.get('upload_date') or ''} | {dur_str} | [{title}]({r['url']}) |\n")

    # INDEX.md
    with (out_dir / "INDEX.md").open("w", encoding="utf-8") as f:
        f.write("# PhysicalAIBuilders YouTube Index\n\n")
        f.write(f"_Last refresh: {dt.datetime.now(dt.timezone.utc).isoformat(timespec='seconds')}_\n\n")
        f.write(f"Total videos indexed across {len(by_creator)} creators: **{len(records)}**\n\n")
        f.write("## Creators\n\n")
        for c in creators:
            handle = c["identifier"]
            safe = re.sub(r"[^A-Za-z0-9_.@-]", "_", handle)
            count = len(by_creator.get(handle, []))
            note = c.get("note", "")
            f.write(f"- [`{handle}`](by-creator/{safe}.md) — {count} videos — {note}\n")


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Headless scraper for Physical-AI / ESP32 YouTube creators.")
    p.add_argument("--creators-file", type=Path, default=DEFAULT_CREATORS)
    p.add_argument("--playlists-file", type=Path, default=DEFAULT_PLAYLISTS,
                   help="YAML with seed playlists; their uploaders are added to the creator pool.")
    p.add_argument("--no-playlist-seed", action="store_true",
                   help="Skip seed-playlist expansion entirely.")
    p.add_argument("--out", type=Path, default=DEFAULT_OUT)
    p.add_argument("--query", type=str, default=None, help="Regex; only videos whose title matches are kept.")
    p.add_argument("--limit", type=int, default=None, help="Max videos per creator (most recent first).")
    p.add_argument("--sleep", type=float, default=2.0, help="Seconds to sleep between creators.")
    p.add_argument("--verbose", action="store_true")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    logging.basicConfig(
        level=logging.DEBUG if args.verbose else logging.INFO,
        format="%(asctime)s %(levelname)s %(message)s",
    )

    args.out.mkdir(parents=True, exist_ok=True)
    error_log = args.out / "errors.log"
    errfh = error_log.open("a", encoding="utf-8")
    for h in log.handlers[:]:
        log.removeHandler(h)
    log.addHandler(logging.StreamHandler(sys.stderr))
    log.addHandler(logging.StreamHandler(errfh))
    log.setLevel(logging.INFO)

    creators = load_creators(args.creators_file)

    if not args.no_playlist_seed:
        seeds = load_seed_playlists(args.playlists_file)
        for s in seeds:
            log.info("expanding seed playlist %s (%s)", s["url"], s.get("note", ""))
            discovered = expand_seed_playlist(s["url"])
            log.info("  discovered %d unique uploaders", len(discovered))
            # merge — de-dupe on identifier
            existing = {c["identifier"] for c in creators}
            for d in discovered:
                if d["identifier"] not in existing:
                    creators.append(d)
                    existing.add(d["identifier"])

    if not creators:
        log.error("No creators found in %s and no seed playlists produced any.", args.creators_file)
        return 2

    query_re = re.compile(args.query, re.IGNORECASE) if args.query else None

    scraped_at = dt.datetime.now(dt.timezone.utc).isoformat(timespec="seconds")
    all_records: list[dict[str, Any]] = []

    for i, c in enumerate(creators):
        handle = c["identifier"]
        note = c.get("note", "")
        log.info("[%d/%d] fetching %s", i + 1, len(creators), handle)
        entries = fetch_one(handle, args.limit)
        kept = 0
        for e in entries:
            if not matches(query_re, e["title"]):
                continue
            all_records.append(
                {
                    "creator_handle": handle,
                    "creator_note": note,
                    "video_id": e["video_id"],
                    "title": e["title"],
                    "url": e["url"],
                    "upload_date": e["upload_date"],
                    "duration_sec": e["duration_sec"],
                    "scraped_at": scraped_at,
                }
            )
            kept += 1
        log.info("  kept %d / %d (query=%r, limit=%s)", kept, len(entries), args.query, args.limit)
        if i < len(creators) - 1 and args.sleep > 0:
            time.sleep(args.sleep)

    write_outputs(all_records, args.out, creators)
    log.info("wrote %d records to %s", len(all_records), args.out)
    errfh.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
