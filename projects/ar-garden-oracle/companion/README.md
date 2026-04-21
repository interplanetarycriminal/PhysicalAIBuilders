# AR Garden Oracle companion — WebBLE + Claude

A Progressive Web App that runs on any recent Android or iOS browser (Chrome / Edge on Android have the best WebBLE support). No app store. No install friction. Pairs with the wand over BLE; asks Claude for a per-plant interpretation.

## Architecture

```
[ phone PWA ]--WebBLE--[ wand ESP32-S3 ]   (spectrum + lux + soil)
       |
       v
[ phone PWA ] --https--> [ Claude API ] --narration--> [ AR overlay ]
                        (prompt cache: species knowledge + spectrum rubric)
       |
       v
[ BLE scan ] --pot MAC--> [ species lookup in config JSON ]
```

## Minimal sample — `oracle.html`

```html
<!doctype html><meta charset=utf-8>
<meta name=viewport content="width=device-width,initial-scale=1">
<title>Garden Oracle</title>
<style>body{font:15px/1.4 system-ui;margin:1em;max-width:32em}</style>
<h1>Oracle</h1>
<button id=scan>Connect wand</button>
<pre id=out></pre>
<script>
const SVC = '6b5f0001-1234-4321-9abc-aura0000garden';
const CHR = '6b5f0002-1234-4321-9abc-aura0000garden';

scan.onclick = async () => {
  const dev = await navigator.bluetooth.requestDevice({
    filters: [{services: [SVC]}]
  });
  const srv = await dev.gatt.connect();
  const svc = await srv.getPrimaryService(SVC);
  const chr = await svc.getCharacteristic(CHR);
  await chr.startNotifications();
  chr.addEventListener('characteristicvaluechanged', async ev => {
    const txt = new TextDecoder().decode(ev.target.value);
    const snap = JSON.parse(txt);
    out.textContent = "Reading: " + txt + "\n\nAsking oracle...";
    const r = await fetch('/oracle', {method:'POST',
      headers:{'content-type':'application/json'},
      body: JSON.stringify(snap)});
    out.textContent += "\n\n" + await r.text();
  });
};
</script>
```

## Server side — `oracle_server.py`

```python
# FastAPI wrapper that asks Claude for a plant-specific interpretation.
# Prompt caches the species knowledge so repeated queries cost pennies.
import os, json
from fastapi import FastAPI, Request
import anthropic

app = FastAPI()
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])

SYSTEM = """You are a gardener's oracle. Given a JSON with AS7341 spectrum
counts, TSL2591 lux, and ADC soil-moisture, return a 3-bullet response:
- What the spectrum suggests about leaf health (chlorophyll, stress).
- Hydration status given current lux and soil ADC.
- ONE specific thing to do today.
Be concise, warm, accurate; do not invent plant identities.
"""

@app.post("/oracle")
async def oracle(req: Request):
    snap = await req.json()
    m = client.messages.create(
        model="claude-sonnet-4-6",
        max_tokens=500,
        system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":SYSTEM}],
        messages=[{"role":"user","content":json.dumps(snap)}],
    )
    return m.content[0].text
```

## Extension ideas

- Use the phone camera + Claude vision to ID the plant species, replacing the BLE pot beacons.
- Store every snapshot to a local DB; graph a plant's spectral trajectory across a season.
- Community corpus — opt-in, aggregated — builds a species × healthy-spectrum index for the whole project.
