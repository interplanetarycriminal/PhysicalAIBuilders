# Ghost Hunter companion — agent narrator

The device is an oracle if and only if it has a narrator. We give an LLM a minimal tool surface that it can call against the ESP32's HTTP endpoints, and a system prompt that tells it to be a *skeptic*, not a showman.

## Architecture

```
[ESP32] exposes HTTP tools: /tools/scan, /tools/breath_rate
                           \---------- JSON ------------/
                                                        |
                                                        v
                                           [Claude agent with tools]
                                                        |
                                                        v
                                           [phone / laptop narration]
```

## Minimal sample — `ghosthunter_agent.py`

```python
# Claude agent with tool-use. Supply the ESP32's LAN IP; let the model call
# /tools/scan and reason over the JSON.
import os, json, httpx
import anthropic

ESP32 = "http://192.168.1.42"
client = anthropic.Anthropic(api_key=os.environ["ANTHROPIC_API_KEY"])

TOOLS = [
    {
      "name": "scan_room",
      "description": "Run a 5-second RF + presence scan of the current room; returns sensor JSON.",
      "input_schema": {"type":"object","properties":{}}
    },
]

def dispatch(name, args):
    if name == "scan_room":
        return httpx.get(f"{ESP32}/tools/scan", timeout=10).text
    raise ValueError(name)

SYSTEM = ("You are a calm paranormal skeptic. Given ambient sensor readings, "
          "explain what they plausibly imply in plain language. Always state "
          "uncertainty. Never claim the supernatural. Prefer the most boring "
          "explanation that fits. Cite the numbers you used.")

def ask(question):
    messages = [{"role":"user","content":question}]
    while True:
        r = client.messages.create(
            model="claude-sonnet-4-6",
            max_tokens=800,
            system=[{"type":"text","cache_control":{"type":"ephemeral"},"text":SYSTEM}],
            tools=TOOLS,
            messages=messages,
        )
        messages.append({"role":"assistant","content":r.content})
        tool_uses = [c for c in r.content if c.type == "tool_use"]
        if not tool_uses:
            return "".join(c.text for c in r.content if c.type == "text")
        tool_results = []
        for tu in tool_uses:
            tool_results.append({
                "type":"tool_result",
                "tool_use_id": tu.id,
                "content": dispatch(tu.name, tu.input),
            })
        messages.append({"role":"user","content":tool_results})

if __name__ == "__main__":
    print(ask("Is anything unusual moving in this room right now?"))
```

## Extension ideas

- Swap Claude for local Ollama (`llama3:8b-instruct`) to keep the whole rig offline.
- Add a mic-level feature (RMS only, no recording) to the tool surface for acoustic corroboration.
- "Skeptic mode" / "Showman mode" system prompts; same hardware, two UX.
