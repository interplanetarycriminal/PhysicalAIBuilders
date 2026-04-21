# FlowState companion — session replay + coach

A WebBLE Progressive Web App that captures the band's 2 s labels during a session, overlays them on a GPS track, and (optionally) asks an LLM to narrate the break-points.

## Architecture

```
[wristband]--BLE(2s labels)-->[phone PWA]
                                  |
                                  +-->[GPS stream]
                                  +-->[local IndexedDB]
                                  +-->[Claude API for narration (optional)]
```

## Minimal sample — `replay.html`

```html
<!doctype html><meta charset=utf-8>
<title>FlowState replay</title>
<style>
  body{font:15px/1.4 system-ui;margin:1em;max-width:40em}
  .bar{height:24px;width:100%;display:grid;grid-auto-flow:column;gap:1px}
  .bar>span{background:#ddd}
  .flow{background:#4a8}.strain{background:#c33}.recov{background:#88b}
</style>
<h1>Session replay</h1>
<div id=bar class=bar></div>
<pre id=out></pre>
<button id=start>Start session</button>

<script>
const SVC='70000001-feee-4001-fa57-f10ab0de0000';
const CHR='70000002-feee-4001-fa57-f10ab0de0000';
const labels=[];

start.onclick = async () => {
  const dev = await navigator.bluetooth.requestDevice({filters:[{services:[SVC]}]});
  const srv = await dev.gatt.connect();
  const svc = await srv.getPrimaryService(SVC);
  const chr = await svc.getCharacteristic(CHR);
  await chr.startNotifications();
  chr.addEventListener('characteristicvaluechanged', e => {
    const [cls, ts] = new Uint8Array(e.target.value.buffer);
    labels.push({cls, ts});
    render();
  });
  navigator.geolocation.watchPosition(p => {
    labels[labels.length-1]=Object.assign(labels[labels.length-1]||{}, {
      lat:p.coords.latitude, lon:p.coords.longitude});
  });
};

function render() {
  bar.innerHTML='';
  for (const l of labels) {
    const s=document.createElement('span');
    s.className = ['recov','flow','strain'][l.cls];
    s.title = JSON.stringify(l);
    bar.appendChild(s);
  }
}
</script>
```

## LLM narration — `narrate.py`

```python
import os, json, sys, anthropic
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
session = json.load(open(sys.argv[1]))  # exported labels+GPS

system = ("You are a kind movement coach. Given a session of 2-second labels "
          "(flow/strain/recovery) and GPS, describe the arc in 3–5 bullets. "
          "Include the timestamp of the first transition from flow to strain. "
          "Never pathologize effort; treat 'strain' as information, not failure.")

m = client.messages.create(
    model="claude-sonnet-4-6",
    max_tokens=500,
    system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":system}],
    messages=[{"role":"user","content":json.dumps(session)}],
)
print(m.content[0].text)
```

## Extension ideas

- Pair session audio (mic on the XIAO) with label — "you exhaled hard here; that correlates with strain entry".
- Coach view (opt-in): WebRTC stream of labels to a remote coach's phone.
- Cohort analytics across many athletes (anonymized) — what features predict flow duration?
