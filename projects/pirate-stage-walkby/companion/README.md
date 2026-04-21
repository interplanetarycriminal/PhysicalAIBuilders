# Pirate Stage companion — the browser scene

**This is where the imagination lives.** The ESP32 is a dumb reporter. This scene is the theatre.

Point a projector (or a big monitor) at a wall. Open `scene.html` in Chrome / Firefox / Edge on any laptop. Type the ESP32-CAM's IP in the text box. Watch passers-by become pirates.

## Architecture

```
[ESP32-CAM]
  |  ws://<ip>:81/  → { seen, x_norm, v_norm, t }  (~6 Hz)
  v
[browser scene.html]
  - maintains a list of "pirate sprites" (upper-body PNGs/SVGs)
  - spawns / retires sprites on `seen` transitions
  - procedural ocean below each sprite
  - couples/crews when spawns overlap in time
  - optional: LLM-generated passing caption
  v
[projector → wall]
```

## `scene.html`

Minimal p5.js scene. Drop into a folder with a `sprites/` subfolder containing `pirate.png` (and any other worlds — `astronaut.png`, `ghost.png`). Swap the sprite; swap the world.

```html
<!doctype html>
<meta charset=utf-8>
<title>Pirate Stage</title>
<style>html,body{margin:0;overflow:hidden;background:#001018;color:#ddd;font:14px/1.4 system-ui}
input{position:fixed;top:8px;left:8px;z-index:2;padding:6px;border-radius:4px;border:0}
canvas{display:block}
</style>
<input id=ip placeholder="ESP32 IP" value="192.168.1.42">
<script src="https://cdn.jsdelivr.net/npm/p5@1.11.1/lib/p5.min.js"></script>
<script>
let ws, latest = {seen:false, x_norm:0.5, v_norm:0, t:0};
const world = { name:"pirate", bg:[2,16,24], wave:[40,100,160], crest:[180,220,240] };
const sprites = {};   // world.name -> p5.Image
const pirates = [];   // active sprites (persons seen)

function preload() {
  sprites[world.name] = loadImage('sprites/' + world.name + '.png',
    img => console.log('sprite ok'),
    err => console.warn('sprite missing; rendering a stand-in rectangle'));
}

function setup() {
  createCanvas(windowWidth, windowHeight);
  imageMode(CENTER);
  document.getElementById('ip').onchange = e => connect(e.target.value);
  connect(document.getElementById('ip').value);
}

function connect(ip) {
  if (ws) ws.close();
  ws = new WebSocket(`ws://${ip}:81/`);
  ws.onmessage = ev => {
    try { latest = JSON.parse(ev.data); onEvent(latest); } catch(e) {}
  };
}

function onEvent(ev) {
  if (ev.seen) {
    const last = pirates[pirates.length - 1];
    if (!last || millis() - last.lastT > 800) {
      pirates.push({
        x: ev.x_norm, v: ev.v_norm, phase: 0, bornT: millis(), lastT: millis(),
        sprite: world.name,
      });
    } else {
      last.x = ev.x_norm; last.v = ev.v_norm; last.lastT = millis();
    }
  }
  // retire stale
  const now = millis();
  for (let i = pirates.length - 1; i >= 0; --i) {
    if (now - pirates[i].lastT > 4000) pirates.splice(i, 1);
  }
}

function drawSea(px, pv, baseY, w, h) {
  const amp = constrain(8 + abs(pv) * 60, 8, 60);
  const period = map(abs(pv), 0, 1, 220, 80);
  const dir = pv >= 0 ? 1 : -1;
  noStroke();
  fill(...world.wave);
  beginShape();
  vertex(px - w/2, height);
  for (let x = px - w/2; x <= px + w/2; x += 4) {
    const y = baseY + sin((x / period) * TWO_PI + frameCount * 0.08 * dir) * amp;
    vertex(x, y);
  }
  vertex(px + w/2, height);
  endShape(CLOSE);
  // crest
  stroke(...world.crest); strokeWeight(1.5); noFill();
  beginShape();
  for (let x = px - w/2; x <= px + w/2; x += 4) {
    const y = baseY + sin((x / period) * TWO_PI + frameCount * 0.08 * dir) * amp;
    vertex(x, y - 2);
  }
  endShape();
}

function drawPirate(p) {
  const x = p.x * width;
  const spriteH = height * 0.45;
  const spriteW = spriteH * 0.7;
  const torsoY = height * 0.45;
  // only upper half of the body is "real"; replace waist-down with sea + hull
  const spr = sprites[p.sprite];
  if (spr && spr.width) image(spr, x, torsoY, spriteW, spriteH);
  else { fill(220); rect(x - spriteW/2, torsoY - spriteH/2, spriteW, spriteH); }

  const baseY = torsoY + spriteH * 0.25;
  drawSea(x, p.v, baseY, spriteW * 2.4, height - baseY);

  // simple boat hull under the pirate
  fill(60, 40, 20);
  beginShape();
  vertex(x - spriteW * 0.7, baseY + 8);
  vertex(x + spriteW * 0.7, baseY + 8);
  vertex(x + spriteW * 0.55, baseY + 60);
  vertex(x - spriteW * 0.55, baseY + 60);
  endShape(CLOSE);
}

function draw() {
  background(...world.bg);
  // subtle horizon
  noStroke(); fill(0, 30, 50); rect(0, height * 0.7, width, 2);
  for (const p of pirates) drawPirate(p);
}

function windowResized() { resizeCanvas(windowWidth, windowHeight); }
</script>
```

## Extension recipes (software-only)

- **Narrator caption.** On spawn, POST `{x_norm, v_norm}` to `/narrate` on your laptop; a small FastAPI wrapper around Claude returns a one-line passer-by caption and the browser prints it beside the pirate.
- **Scene morph by time-of-day.** At dawn, pastel sails; midday, calm doldrums; dusk, storm; night, lanterns and constellations.
- **Crowd physics.** Two passers-by moving the same direction = sync their wave phase; two opposite directions = a choppy "collision" wave.
- **Audio layer.** Use the Web Audio API; speed controls sea-shanty BPM; presence triggers seagulls.
- **Archive mode.** Save one PNG of each unique pass to a `log/` folder; at midnight, a script composes them into a "ship's log" collage.
- **Swap the world.**
  - `world.name = 'astronaut'` — pirates become astronauts; sea → starfield with drifting asteroids.
  - `world.name = 'ghost'` — upper body is a spectral silhouette; seas → mist.
  - `world.name = '1920s'` — art-deco borders, sepia palette, a jazz loop.

## Tiny caption server (`narrate.py`) — optional

```python
# Optional: a one-line per passer-by caption generator.
import os, json, anthropic
from fastapi import FastAPI, Request
app = FastAPI()
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
SYSTEM = ("You are a playful pirate-lore narrator. For each passer-by, invent a "
          "one-line name/role (e.g. 'Captain Bess, returning from the butcher'). "
          "Never identify real people; always fictional.")
@app.post("/narrate")
async def narrate(req: Request):
    s = await req.json()
    m = client.messages.create(
        model="claude-haiku-4-5-20251001",
        max_tokens=60,
        system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":SYSTEM}],
        messages=[{"role":"user","content":json.dumps(s)}],
    )
    return m.content[0].text
```

## See also

- [`../../framework/software-as-imagination.md`](../../framework/software-as-imagination.md) — the manifesto.
- [`../../framework/riff-book.md`](../../framework/riff-book.md) — 60+ similar one-liner riffs.
- [`../../personas/11-creative-remix.md`](../../personas/11-creative-remix.md) — ten personas who think this way as a practice.
