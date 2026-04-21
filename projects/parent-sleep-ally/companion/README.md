# Parent Sleep Ally companion — morning line generator

One job: take the overnight rollups and write one honest sentence for the e-paper at 06:30.

## Architecture

```
[ESP32] --MQTT per-minute--> [broker] --subscribe--> [sleep_ally_service.py]
                                                              |
                                                              v
                                     06:00 each day → [Claude API] → one-sentence
                                                              |
                                                              v
                                 [MQTT sleep-ally/<room>/morning-line] → e-paper
```

## Minimal sample — `sleep_ally_service.py`

```python
import os, json, time, collections
import paho.mqtt.client as mqtt
import anthropic

ROOM = "nursery"
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
buffer = collections.deque(maxlen=60 * 14)   # 14 hours of per-minute rows

def on_msg(_c, _u, m):
    if m.topic.endswith("/minute"):
        buffer.append(json.loads(m.payload))

def morning_line():
    if not buffer:
        return "No data overnight — check the ally."
    # Compact features for the prompt: stretch of presence, CO2 peaks, loud events, coldest minute.
    last_night = list(buffer)
    loud_total = sum(r["loud"] for r in last_night)
    co2_max = max(r["co2"] for r in last_night)
    co2_avg = sum(r["co2"] for r in last_night) / len(last_night)
    presence_frac = sum(1 for r in last_night if r["pres"] > 120) / len(last_night)

    system = ("You are a warm, careful, honest parent-friend writing ONE sentence "
              "(maximum 18 words) for an e-paper at 06:30. Never alarm; never minimize. "
              "Describe the night factually, tell the parent what you'd tell a friend.")

    snap = {
        "co2_peak_ppm": round(co2_max),
        "co2_avg_ppm": round(co2_avg),
        "loud_events": loud_total,
        "presence_fraction": round(presence_frac, 2),
    }
    m = client.messages.create(
        model="claude-sonnet-4-6",
        max_tokens=80,
        system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":system}],
        messages=[{"role":"user","content":json.dumps(snap)}],
    )
    return m.content[0].text.strip().replace("\n", " ")

def main():
    c = mqtt.Client()
    c.on_message = on_msg
    c.connect("localhost")
    c.subscribe(f"sleep-ally/{ROOM}/minute")
    c.loop_start()

    last_emit_day = None
    while True:
        t = time.localtime()
        if t.tm_hour == 6 and t.tm_min == 0 and t.tm_yday != last_emit_day:
            line = morning_line()
            c.publish(f"sleep-ally/{ROOM}/morning-line", line, retain=True)
            last_emit_day = t.tm_yday
        time.sleep(20)

if __name__ == "__main__":
    main()
```

## Extension ideas

- Two rooms, two allies, a combined "household night" line.
- Parent's bed unit subscribes to the alert topic and renders haptic instead of text.
- Home Assistant integration: expose the rollups as sensors, let automations tune the HVAC.
