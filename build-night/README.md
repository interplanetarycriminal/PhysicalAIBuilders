# Build Night — the kit reality layer

> Most of this repo is **aspirational**: a 1→100 ladder of what Physical AI *can* be, with
> flagship builds that reach for thermal cameras, mmWave radar, LoRa mesh and solar field nodes.
> This section is the opposite. It is the **ground truth of one real room on one real night** —
> exactly the hardware on the bench, its working limitations, and the handful of things you can
> actually ship in 2.5 hours with it.

If you are running a session, start here. If you are reading the encyclopedia or the flagship
projects and wondering *"can I build this with the gear in front of me tonight?"* — this section
is the honest answer.

## The night in one sentence

Teams of two, their own laptops, an ESP32 over USB, a sensor grabbed from a Keyestudio kit, and
**Perplexity + Claude Code** to ship a working proof-of-concept fast. No soldering.

## The four reference pages

| Page | What it is |
|---|---|
| [`inventory.md`](inventory.md) | **What we physically have.** The board pools and every Keyestudio 42-in-1 module, with voltage and difficulty flags. The source of truth for "is this part in the kit?". |
| [`safety-and-gpio.md`](safety-and-gpio.md) | **The working limitations.** The 5 safety rules, the 3.3V law, brownout discipline, and the safe GPIO maps for the classic ESP32 and the S3 (including the S3's missing GPIO22). |
| [`templates-catalogue.md`](templates-catalogue.md) | **The menu.** Part → library → safe pins → code pointer, plus the crowd-pleaser combos. What you read to a team so they know exactly what to hand Claude Code. |
| [`../MAY29th_README_PhysicalAIBuilders.md`](../MAY29th_README_PhysicalAIBuilders.md) | **The facilitator pack**, hand-it-out-as-is. The full self-contained playbook (run-of-show, escalation, playbook) these pages are distilled from. |

## The builds you can actually ship tonight

Six full guides, kit-only, no parts beyond what's on the bench. They live in `projects/` with a
`kit-` prefix and sit at the bottom of [the ladder](../framework/learning-levels.md) (Levels 10–20):

- [`kit-parking-sensor`](../projects/kit-parking-sensor/) — ultrasonic + buzzer, beeps faster as you approach. (L10)
- [`kit-sound-reactive-lights`](../projects/kit-sound-reactive-lights/) — sound sensor drives the 6812 RGB. (L10)
- [`kit-tripwire-alert`](../projects/kit-tripwire-alert/) — PIR + relay + laser/buzzer intruder line. (L10→20)
- [`kit-weather-station`](../projects/kit-weather-station/) — DHT11 temp/humidity on the LCD. (L20)
- [`kit-joystick-arcade`](../projects/kit-joystick-arcade/) — joystick + 8×8 dot matrix mini-game. (L20)
- [`kit-rfid-doorman`](../projects/kit-rfid-doorman/) — RFID card opens a servo "door". (L20)

## The build loop (read this to the room)

**Idea → Perplexity scopes it → Claude Code builds and flashes it → test on hardware → paste the
exact error back → iterate → working POC.**

Treat Claude Code like an eager intern: fast and helpful, not error-free first try. The
highest-leverage move in the loop is **pasting the exact, verbatim compiler or serial error back**
— not a summary. Every time you hand Claude Code a task, give it: the exact board / FQBN, the exact
part number + library to install, one plain sentence of intent, the wiring you actually used, and
the full error text. See the [facilitator pack](../MAY29th_README_PhysicalAIBuilders.md) §3 for the
full workflow and the `arduino-cli` / PlatformIO flash commands.

## The stretch goal (for fast teams on the S3)

The 5-strong **ESP32-S3** pool is the one to try the "AI agent reacts physically" pattern on, over
**Bluetooth LE**. The open-source `anthropics/claude-desktop-buddy` (Arduino/ESP32, Nordic UART
BLE) is a desk-pet that physically reacts to Claude Code's permission prompts — sleeping when idle,
waking on a session, getting visibly impatient when an approval is waiting. A full port is more than
one night; use it as the **pattern**, not a tonight deliverable. This is exactly the
[Level 80 "Hardware + Agent"](../framework/learning-levels.md) rung, brought within reach.

## How this fits the rest of the repo

The May 29 kit fully covers **Levels 1–20** of the ladder. The exotic
[flagship projects](../projects/) (Levels 30–100) each now carry a *Hardware reality* banner naming
the parts they need beyond this kit and the nearest kit-only cousin to build tonight. The
[hardware encyclopedia](../hardware/) flags which categories the kit actually touches. Nothing here
contradicts the bench anymore — the vision and the reality are clearly labelled and cross-linked.
