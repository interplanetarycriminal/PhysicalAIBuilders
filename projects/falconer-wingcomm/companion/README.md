# WingComm companion — phone bridge + post-flight log

The glove stores every flight to its SD; the companion pulls it to the phone after landing, draws the track on a map, and optionally asks Claude for a post-flight summary.

## Architecture

```
[bird LoRa]--5Hz-->[glove console]--SD/log-->[phone, pulled over SoftAP]
                                                     |
                                                     +-->[map overlay]
                                                     +-->[LLM post-flight log]
```

## Minimal sample — flight summary prompt

```python
import os, json, sys, anthropic
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])
flight = json.load(open(sys.argv[1]))  # list of BirdPacket-dicts

system = (
 "You are a field-log assistant for falconry. Given a list of packets (lat, "
 "lon, alt, heading, stoop flags), produce a short post-flight log: takeoff, "
 "peak altitude, number of stoops, approximate pursuit duration, and a "
 "one-line note on the shape of the flight. Don't invent prey; you can't see."
)

m = client.messages.create(
    model="claude-sonnet-4-6",
    max_tokens=400,
    system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":system}],
    messages=[{"role":"user","content":json.dumps(flight)}],
)
print(m.content[0].text)
```

## Extension ideas

- Offline basemap tiles so glove map works in the field without cell service.
- Community "best flights" ranked by ascent + stoop power, posted opt-in to a public gallery.
- Integration with iNaturalist or your training journal.
