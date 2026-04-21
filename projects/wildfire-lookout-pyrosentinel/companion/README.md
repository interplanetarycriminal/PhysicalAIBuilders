# PyroSentinel companion — gateway, dashboard, SitRep agent

## Architecture

```
[ridge nodes ×N]--LoRa-->[gateway laptop]--Timescale-->[Grafana + Mapbox dashboard]
                                       \--every 30min-->[Claude SitRep agent]--email/SMS-->ops
```

## Ops dashboard

Three panels (Grafana + Mapbox via a JSON-panel plugin):

1. **Valley map** — one marker per node, shaded confidence sector from each node's `plume_bearing_deg`. Where two sectors intersect at elevated `plume_score`, that's your likely ignition point.
2. **Time-series** — PM2.5, plume_score, wind, RH across all nodes.
3. **Node health** — battery voltage, last-seen, FOTA status, firmware hash.

## SitRep agent

```python
# Every 30 min, summarize rollups into an ICS-201-ish SitRep.
import os, json, datetime, anthropic
from psycopg import connect

db = connect(os.environ["PG_URL"])
rows = db.execute(
    "SELECT node_id, ts, plume_score, plume_bearing_deg, pm25, wind_mps, wind_dir_deg "
    "FROM sentinel WHERE ts > now() - interval '30 minutes' ORDER BY ts"
).fetchall()

client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
SYSTEM = (
 "You are a terse, correct wildland-fire situation-report drafter. "
 "Given 30 minutes of multi-node sensor data, produce a SitRep with "
 "sections: 1) Current Situation 2) Smoke/Plume Indicators 3) Wind & "
 "Atmosphere 4) Confidence Level 5) Recommended Next Look. "
 "Never invent a fire name, location, or casualty count. Always hedge."
)
m = client.messages.create(
    model="claude-sonnet-4-6",
    max_tokens=900,
    system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":SYSTEM}],
    messages=[{"role":"user","content":json.dumps([list(r) for r in rows], default=str)}],
)
print(m.content[0].text)
```

## Safety design notes

- All actions from the agent output are *for human review*. Do not wire it to any automated dispatch.
- The SitRep is an **aid**, not an authority.

## Extension ideas

- Integrate with the local National Weather Service HRRR forecast to add meteorological context.
- Partner with AlertWildfire / public camera feeds; the agent cross-references camera imagery with the node network for corroboration.
- Public-facing "haze forecast" product (carefully scoped; node locations withheld).
