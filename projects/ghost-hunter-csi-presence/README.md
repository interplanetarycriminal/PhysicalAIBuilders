# Ghost Hunter — CSI + mmWave presence fusion

> **Level:** 80 — Hardware + Agent. Weeks–months. ~$350.
> **Tribe:** Strange & archetypal (#81)
> **One-line vibe:** a device that knows if there's a person behind the wall — without a camera, without a microphone — and asks an LLM to narrate the weird.

## The asymmetry

You walk into a "haunted" room and can tell the skeptic's question calmly: **"There is someone moving in the next room; their approximate breath rate is 17/min; CSI and mmWave both agree."** Ordinary phones tell you nothing. This device, paired with an LLM narrator, converts raw radio-noise into a clean story.

## The two-line spec

```
HARDWARE:  ESP32 DevKitC (CSI-capable) + HLK-LD2410B + SSD1306 OLED + optional INMP441 + battery
SOFTWARE:  ESP-IDF firmware exposing a tool-surface + local Ollama / Claude agent narrator
```

## Bill of materials

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 1 | ESP32 DevKitC (original — CSI is best supported here) | MCU | Adafruit | $7 |
| 1 | HLK-LD2410B | mmWave presence | AliExpress | $5 |
| 1 | SSD1306 128×64 | Status | AliExpress | $5 |
| 1 | INMP441 (optional) | Ambient acoustic | Adafruit | $6 |
| 1 | 2× 18650 + holder + BMS | Power | 18650BatteryStore | $25 |
| 1 | Raspberry Pi 4 / mini PC | LLM host (Ollama) | Pi shop | $65 (optional) |
| 1 | Enclosure + dial + button | Form | 3D print | — |

**Total:** ~$50 + optional LLM host.

## Wiring

```
ESP32 DevKitC
 ├─ Serial2 (GPIO16/17) ── LD2410B TX/RX
 ├─ SDA/SCL (21/22) ────── SSD1306
 └─ I²S (25/26/33) ─────── INMP441 (optional)

Power: battery → MCP73871 (solar-aware Li charger) → 3V3 / 5V buck → ESP32 + peripherals
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). ESP-IDF (not Arduino) — this build exercises real CSI APIs.

- Enables Wi-Fi CSI via `esp_wifi_set_csi_rx_cb`; parses subcarrier amplitude into a rolling spectrogram.
- Fuses CSI-variance-over-time with LD2410B `presence`/`motion` flags + rough range.
- Exposes a **tool surface** over HTTP (JSON): `/tools/scan`, `/tools/breath_rate`, `/tools/narrate_prompt`.

## Companion software

See [`companion/README.md`](companion/README.md). The interesting half. A local agent (Ollama running `llama3:8b` or Claude via API) is given this ESP32's tool surface and a system prompt that says: *"You are a paranormal skeptic. Use the tools to explain the room."* The agent calls `/tools/scan`, reads the JSON, and narrates.

## Ethics of this asymmetry

- This is the most ethically loaded build in this repo because it perceives people. **Use on your own premises, on guests with clear consent, or for skeptic-community demos.**
- Don't point it at neighbors' walls.
- Persist nothing raw; only persist the LLM-narrated episode (summary text) if the user saves it.

## Extension paths (climbing the ladder)

- **+10 (level 90):** add a Person Sensor (Useful Sensors) for camera-free face-count corroboration; fusion widens.
- **+20 (level 100):** deploy a multi-node CSI array across a flat; your "ghost" is trilaterated.

## Credits & inspiration

Hernandez & Bulut's CSI-for-presence papers, the Meshtastic community for low-bandwidth sensibility, and every skeptic podcast that ever deserved better tools.
