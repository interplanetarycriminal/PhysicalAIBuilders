# FermentDoc companion — dashboard + LLM batch notebook

Two small pieces. Both run on the same laptop / NAS / Pi.

## Architecture

```
[ESP32] --MQTT--> [Mosquitto broker] --sub--> [Flask dashboard]
                                         \-> [SQLite batch store]
[batch end] ----JSON-----> [Claude API] -> [batch_<id>.md]
```

## Minimal sample — `dashboard.py`

```python
# Tiny MQTT → SQLite → Chart.js dashboard. 50 lines.
from flask import Flask, render_template_string, jsonify
import paho.mqtt.client as mqtt
import sqlite3, threading, time

DB = "fermentdoc.sqlite"
app = Flask(__name__)

def store(batch, metric, v):
    with sqlite3.connect(DB) as c:
        c.execute(
            "CREATE TABLE IF NOT EXISTS reads(ts INT, batch TEXT, metric TEXT, v REAL)"
        )
        c.execute("INSERT INTO reads VALUES (?,?,?,?)", (int(time.time()), batch, metric, v))

def on_msg(_client, _u, msg):
    # topic format: fermentdoc/<batch>/<metric>
    parts = msg.topic.split("/")
    if len(parts) != 3: return
    try: v = float(msg.payload)
    except ValueError: return
    store(parts[1], parts[2], v)

cli = mqtt.Client()
cli.on_message = on_msg
cli.connect("localhost", 1883)
cli.subscribe("fermentdoc/#")
threading.Thread(target=cli.loop_forever, daemon=True).start()

@app.get("/data/<batch>")
def data(batch):
    with sqlite3.connect(DB) as c:
        rows = c.execute(
            "SELECT ts, metric, v FROM reads WHERE batch=? ORDER BY ts", (batch,)
        ).fetchall()
    out = {}
    for ts, m, v in rows:
        out.setdefault(m, []).append([ts*1000, v])
    return jsonify(out)

@app.get("/")
def index():
    return render_template_string("""
      <!doctype html><meta charset=utf-8>
      <title>FermentDoc</title>
      <canvas id=c width=720 height=320></canvas>
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
      <script>
        fetch('/data/batch-001').then(r=>r.json()).then(d=>{
          new Chart(c, {type:'line', data:{datasets:
            Object.entries(d).map(([k,v])=>({label:k,data:v.map(p=>({x:p[0],y:p[1]}))}))},
            options:{scales:{x:{type:'time'}}}});
        });
      </script>
    """)

if __name__ == "__main__":
    app.run("0.0.0.0", 8000)
```

## LLM batch notebook — `batch_notebook.py`

```python
# Read SQLite for a finished batch; ask Claude for tasting notes + next-batch tweak.
# Prompt caching keeps it cheap across batches.
import os, json, sqlite3, sys
import anthropic

batch = sys.argv[1]   # e.g. "batch-007"
with sqlite3.connect("fermentdoc.sqlite") as c:
    rows = c.execute(
        "SELECT ts, metric, v FROM reads WHERE batch=? ORDER BY ts", (batch,)
    ).fetchall()

series = {}
for ts, m, v in rows:
    series.setdefault(m, []).append({"t": ts, "v": v})

client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
msg = client.messages.create(
    model="claude-sonnet-4-6",
    max_tokens=1000,
    system=[{
        "type": "text",
        "cache_control": {"type": "ephemeral"},
        "text": (
            "You are a brewer's notebook. Given a JSON of batch telemetry and "
            "the recipe, return: tasting-note prediction, fermentation health "
            "assessment, and ONE concrete tweak for the next batch."
        ),
    }],
    messages=[{
        "role": "user",
        "content": json.dumps({"batch": batch, "series": series}, default=str),
    }],
)
print(msg.content[0].text)
```

## Extension ideas

- Add a Tilt Hydrometer BLE scanner to the ESP32 (NimBLE) and publish `gravity`.
- Per-batch CSV export for BeerSmith / BrewFather imports.
- Fleet mode: publish all batches from multiple brewers to a shared community dashboard (opt-in).
