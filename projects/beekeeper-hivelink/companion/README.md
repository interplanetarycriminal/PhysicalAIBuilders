# HiveLink companion — gateway + forecast notebook

## Architecture

```
[hive ESP32 + LoRa]--15min--> [Pi gateway]--InfluxDB-->[Grafana dashboard]
                                          |
                                          +--weekly cron--> [Claude forecast notebook] --> e-mail / Telegram
```

## Gateway (Raspberry Pi)

A tiny Python listener for LoRa packets (if using a Pi with an SX126x HAT) or ChirpStack for a proper LoRaWAN setup. Parses the `HiveSummary` struct, writes to InfluxDB, and fans out MQTT topics for the dashboard.

```python
# gateway_lora.py — minimal RN2483 / SX1262 HAT listener example
import struct, time, os
from influxdb_client import InfluxDBClient, Point
# ... your preferred LoRa lib

FMT = "<I f f f f f f f f f f"   # HiveSummary packet

client = InfluxDBClient(url="http://localhost:8086", token=os.environ["INFLUX_TOKEN"], org="bees")
write = client.write_api()

def on_packet(raw_bytes):
    if len(raw_bytes) < struct.calcsize(FMT): return
    t = struct.unpack(FMT, raw_bytes[:struct.calcsize(FMT)])
    (seq, w, wderiv, tbc, tbsd, co2, pm25, voc, blo, bhi, bat) = t
    p = (Point("hive").field("weight_kg", w).field("dkg_per_min", wderiv)
         .field("tbc", tbc).field("tbsd", tbsd).field("pm25", pm25)
         .field("voc", voc).field("band_low", blo).field("band_high", bhi)
         .field("batt", bat))
    write.write(bucket="hive", record=p)
```

## Weekly forecast notebook — `forecast.py`

```python
import os, json, datetime, anthropic
from influxdb_client import InfluxDBClient

api = InfluxDBClient(url="http://localhost:8086", token=os.environ["INFLUX_TOKEN"], org="bees").query_api()
rows = api.query_data_frame(
    'from(bucket:"hive") |> range(start: -30d) |> aggregateWindow(every: 1h, fn: mean)'
)
summary = rows.to_dict(orient="records")

system = ("You are an opinionated bee-ops assistant. Given the last 30 days "
          "of per-hour summaries (weight, weight derivative, brood temp stability, "
          "PM2.5, VOC, acoustic low/high band), write: "
          "1) a swarm-risk forecast (low/med/high) with a candidate date window "
          "2) a queen-health signal read "
          "3) ONE inspection recommendation for this week. "
          "Never invent a colony size; never instruct pesticide use.")

client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
m = client.messages.create(
    model="claude-sonnet-4-6",
    max_tokens=800,
    system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":system}],
    messages=[{"role":"user","content":json.dumps(summary, default=str)}],
)
print(m.content[0].text)
```

## Extension ideas

- Multi-hive differential — compare acoustic fingerprint across hives to flag the weird one.
- Queen-event replay — when weight drops > 1 kg, scrub the prior 48 h of acoustic and expose it as a time-lapse.
- Apiary-map dashboard for a city-wide beekeeping society (opt-in, privacy-respecting).
