# Party Icebreaker — AuraLens badge

> **Level:** 10 — First Sense. A Saturday afternoon. $25.
> **Tribe:** Spies & Shadows (#6 Carmen Sandiego, #10 Q)
> **One-line vibe:** a badge that gently lights up when you're near someone who shares one of your interests.

## The asymmetry

Ordinary small-talk at a party fails because nobody knows who cares about what. AuraLens lets two strangers discover a shared interest *before* either of them has to commit to speaking. It's not matchmaking; it's an ambient cue, like eye contact with a stranger who reads the same book. The asymmetry: the wearer knows — from the color of their own badge — that someone nearby shares something non-trivial with them.

## The two-line spec

```
HARDWARE:  XIAO ESP32-C3 + SSD1306 OLED + 1× NeoPixel + 3V LiPo (~150 mAh)
SOFTWARE:  on-device BLE advertise/scan + a phone webview to pick your interests
```

## Bill of materials

| Qty | Part | Role | Vendors | Approx $ |
|---|---|---|---|---|
| 1 | Seeed XIAO ESP32-C3 | MCU + BLE | Seeed, DigiKey, Mouser | $5 |
| 1 | 0.91" SSD1306 128×32 OLED (I²C) | Name display | Adafruit, AliExpress | $5 |
| 1 | WS2812 NeoPixel Mini | Aura color | Adafruit, AliExpress | $1 |
| 1 | 150 mAh LiPo + JST-PH | Power | Adafruit | $6 |
| 1 | Slide switch | Power disable | DigiKey | $1 |
| 1 | Lanyard + 3D-printed enclosure | Wearable form | Print local | $5 |
| — | Consent LED (can be the NeoPixel itself in white) | Privacy signal | — | — |

**Total:** ~$23

## Wiring

```
XIAO ESP32-C3
 ├─ 3V3 ──► OLED VCC, NeoPixel VCC
 ├─ GND ──► OLED GND, NeoPixel GND, LiPo −
 ├─ SDA (D4) ─► OLED SDA
 ├─ SCL (D5) ─► OLED SCL
 ├─ D2  ────► NeoPixel DIN (via 330 Ω)
 └─ battery pad ◄── LiPo + (via slide switch)
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino IDE with Seeed XIAO ESP32-C3 board package, `Adafruit_NeoPixel`, `Adafruit_SSD1306`, `NimBLEDevice`.

Key choices:

- Each badge **advertises a BLE service** with a 32-bit "interest vector" (hashed tags from your 6 favourite tags) in the `Manufacturer Data` field — no PII.
- Each badge also **scans passively**; when another badge's interest vector shares ≥1 bit with ours, we pulse our NeoPixel a color derived from the shared bit.
- Interest vectors rotate every 5 min so passive MAC tracking can't follow you home.

## Companion software

See [`companion/README.md`](companion/README.md). A tiny phone webpage served by the ESP32 in SoftAP mode on first boot; you pick your interests from a list; it writes a 32-bit hash to NVS. No server, no cloud.

## Ethics of this asymmetry

- The badge is **opt-in** and **visible** (wearable; LED always on while broadcasting).
- No MAC persistence (interest vector + adv address rotate every 5 min).
- Interest tags are local, chosen from a public list; no free-text, no sensitive-category tags.
- "Do not ever contact me" escape hatch: turn the badge off; you're invisible.

## Extension paths (climbing the ladder)

- **+10 (level 20):** add a tiny FastAPI server per party that aggregates colors into a "vibe map" of the room (opt-in).
- **+20 (level 30):** add a gesture (IMU) to request a "hi" — sends a one-time haptic to the matched badge.
- **+30 (level 40):** LLM-generated conversation starter on the OLED when match fires ("you both like Sergio Leone westerns").

## Credits & inspiration

BLE beacon conference badges (DEF CON, 36C3). Our twist: interest-vector hashing instead of identity; short rotation; ambient color instead of text.
