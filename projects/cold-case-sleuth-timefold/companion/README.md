# TimeFold companion — ingest, embeddings, agent

Three services on the gateway, each doing one job well.

## 1. Ingest — `ingest.py`

Subscribes to `timefold/<node>/minute`, writes rollups to TimescaleDB.

```python
import os, json, paho.mqtt.client as mqtt
from psycopg import connect
db = connect(os.environ["PG_URL"])
db.execute("""
  CREATE TABLE IF NOT EXISTS minute (
    ts timestamptz DEFAULT now(),
    node text, pres int, mot int, rms real, t real, rh real, p real
  );
  SELECT create_hypertable('minute','ts', if_not_exists => true);
""")
def on(_c,_u,m):
    node = m.topic.split("/")[1]
    j = json.loads(m.payload)
    db.execute(
      "INSERT INTO minute(node,pres,mot,rms,t,rh,p) VALUES(%s,%s,%s,%s,%s,%s,%s)",
      (node, j["pres"], j["mot"], j["rms"], j["t"], j["rh"], j["p"]),
    )
    db.commit()
c = mqtt.Client(); c.on_message=on; c.connect("localhost"); c.subscribe("timefold/#"); c.loop_forever()
```

## 2. Embeddings + anomaly scoring — `fold.py`

Hourly cron (or APScheduler). Pull the last hour per-node, compute a 128-dim embedding, store in a vector column.

```python
import os, numpy as np, pandas as pd
from psycopg import connect
from datetime import datetime, timedelta

db = connect(os.environ["PG_URL"])
def hour_embedding(df: pd.DataFrame) -> np.ndarray:
    # Deliberately simple — swap in a learned auto-encoder later.
    feats = []
    for col in ("pres","mot","rms","t","rh","p"):
        s = df[col].astype(float)
        feats += [s.mean(), s.std(), s.quantile(0.1), s.quantile(0.9),
                  s.diff().abs().mean()]
    # Pad to 128
    a = np.array(feats, dtype=np.float32)
    return np.pad(a, (0, 128 - len(a)))

end = datetime.utcnow().replace(minute=0, second=0, microsecond=0)
start = end - timedelta(hours=1)
rows = pd.read_sql(
   "SELECT * FROM minute WHERE ts >= %s AND ts < %s", db, params=(start, end)
)
for node, g in rows.groupby("node"):
    v = hour_embedding(g).tolist()
    db.execute(
      "INSERT INTO hourly_emb(ts_start, node, vec) VALUES (%s,%s,%s)",
      (start, node, v),
    )
db.commit()
```

## 3. The investigator agent — `detective.py`

```python
# Claude agent with tools against the DB and the ESP32 nodes.
import os, json, httpx, numpy as np, anthropic
from psycopg import connect
from datetime import datetime, timedelta

client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
db = connect(os.environ["PG_URL"])

TOOLS = [
  {"name":"list_anomalies","description":"Top-N anomalous hours over the last N days.",
   "input_schema":{"type":"object","properties":{
       "days":{"type":"integer","default":7},"k":{"type":"integer","default":5}},"required":[]}},
  {"name":"playback","description":"Raw minute rollups around an anomaly.",
   "input_schema":{"type":"object","properties":{
       "ts":{"type":"string"},"window_min":{"type":"integer","default":30},
       "node":{"type":"string"}},"required":["ts"]}},
]

def tool_list_anomalies(days=7, k=5):
    end = datetime.utcnow()
    start = end - timedelta(days=days)
    rows = db.execute("SELECT ts_start, node, vec FROM hourly_emb WHERE ts_start > %s", (start,)).fetchall()
    if not rows: return []
    V = np.array([r[2] for r in rows])
    mu = V.mean(axis=0); sd = V.std(axis=0) + 1e-6
    scores = np.linalg.norm((V - mu) / sd, axis=1)
    order = np.argsort(-scores)[:k]
    return [{"ts": str(rows[i][0]), "node": rows[i][1], "score": float(scores[i])} for i in order]

def tool_playback(ts, window_min=30, node=None):
    clause = "AND node = %s" if node else ""
    params = (ts, ts, window_min)
    if node: params = params + (node,)
    return db.execute(
      f"SELECT ts,node,pres,mot,rms,t,rh,p FROM minute WHERE ts BETWEEN %s::timestamptz - interval '%s minutes' AND %s::timestamptz + interval '%s minutes' {clause} ORDER BY ts",
      (ts, window_min, ts, window_min, *( (node,) if node else ()))
    ).fetchall()

def dispatch(name, args):
    if name == "list_anomalies": return json.dumps(tool_list_anomalies(**args))
    if name == "playback": return json.dumps(tool_playback(**args), default=str)
    raise ValueError(name)

SYSTEM = (
 "You are a patient investigator with a sensor-network detective kit. "
 "Start from the most boring explanations (a window opened, a dog on the couch). "
 "Call tools; cite specific numbers; hedge confidence. If an anomaly has no "
 "boring explanation, draft a hypothesis. Never claim certainty."
)

def ask(q):
    messages=[{"role":"user","content":q}]
    while True:
        r = client.messages.create(
            model="claude-sonnet-4-6", max_tokens=1200,
            system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":SYSTEM}],
            tools=TOOLS, messages=messages,
        )
        messages.append({"role":"assistant","content":r.content})
        tu = [c for c in r.content if c.type == "tool_use"]
        if not tu: return "".join(c.text for c in r.content if c.type == "text")
        tr = [{"type":"tool_result","tool_use_id":t.id,"content":dispatch(t.name,t.input)} for t in tu]
        messages.append({"role":"user","content":tr})

if __name__ == "__main__":
    print(ask("Summarize this week's weirdest hour and hypothesize why."))
```

## 4. Reviewer UI

A small Flask / FastAPI app with a timeline and an agent-conversation pane. Accept / reject buttons write labeled anomalies back to `hourly_emb` as ground truth that improves future scoring.

## Deliverables (the "research-grade" piece)

- `BASELINE.md` — the characterization of what "normal" looks like for this deployment.
- `ETHICS.md` — the consent, retention, and disclosure design, signed by every person living/working in the sensed space.
- `DATASET.md` — schema and anonymization decisions for any data you share.
- `PAPER_DRAFT.md` — what you learned that was worth writing down.

You are now off the top of the ladder. The next rungs are yours to invent.
