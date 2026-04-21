# Companion software — {Project name}

The software twin. Pick one (or more) of the four partners in [`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md):

1. On-device (the firmware itself, in `../firmware/`).
2. Edge-local (Home Assistant / Node-RED / local Flask app on your LAN).
3. Cloud (FastAPI / Supabase / a web app).
4. Agentic / LLM (Claude via MCP-style tools; local Ollama).

## Architecture

```
[ ESP32 ] --{transport}--> [ companion ] --{interface}--> [ human or agent ]
```

## Minimal sample

{One small code sample that proves the companion works end-to-end with the firmware.}

## Extension ideas

- Add an agent loop that can act on the hardware (see [`../../../capabilities/reason.md`](../../../capabilities/reason.md)).
- Multi-device fan-out (see [`../../../capabilities/represent.md`](../../../capabilities/represent.md)).
