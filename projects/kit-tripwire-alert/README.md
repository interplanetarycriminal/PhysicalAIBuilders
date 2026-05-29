# Kit Tripwire Alert

> **Level:** 10 → 20 — First Sense climbing to Connected Object. One session. Kit-only.
> **Tribe:** Spies & Shadows
> **One-line vibe:** an invisible line across a doorway that *knows* when it's crossed.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. A
> [build-night](../../build-night/) crowd-pleaser. The relay is a **5V** part — power its coil from
> the expansion board's 5V rail, never the 3.3V pin. See
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## The asymmetry

You can't watch a doorway forever; a PIR can, and it never blinks. The wielder knows the instant a
space was entered — and the relay lets that knowledge *act* on the world (trip a lamp, a siren, a
fan) instead of just logging it. This is the smallest honest version of "presence detection drives a
physical response."

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + PIR Motion + 5V Relay + Laser (armed beam) / Buzzer
SOFTWARE:  on-device interrupt + latched alarm window; serial event log; optional push/web
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | PIR Motion Sensor | Presence (digital) | Keyestudio 42-in-1 kit | in kit |
| 1 | 5V Relay Module | Switch a real load (5V) | Keyestudio 42-in-1 kit | in kit |
| 1 | Laser Module (or Active Buzzer) | Armed indicator / alarm | Keyestudio 42-in-1 kit | in kit |
| 1 | ESP32-IO Expansion Board + 6×AA holder | 5V rail for the relay coil | Keyestudio 42-in-1 kit | in kit |

**Total:** ~$0 beyond the kit.

## Wiring

```
PIR Motion                  ESP32 (classic)
 OUT ──► GPIO13             (digital, interrupt-capable)
 V   ──► 3V3 (or 5V per module; signal is 3.3V-safe)
 G   ──► GND

5V Relay
 IN  ──► GPIO4              (trigger from a GPIO)
 V   ──► 5V (expansion/AA rail)        ⚠ not the 3.3V pin
 G   ──► GND (common ground)
 (switch the laser / a lamp / the kit fan on the COM/NO terminals — avoid mains)

Laser Module
 SIG ──► GPIO18             (armed beam; do NOT point at eyes)
 V   ──► 3V3   G ──► GND
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework; no extra library (PIR via an
interrupt, relay/laser via `digitalWrite`).

Key choices:

- PIR fires a hardware **interrupt** (`RISING`) so no motion is missed between loop iterations.
- A latched **alarm window** (3 s) keeps the relay on briefly after each trip, so a quick walk-through
  still produces a visible response.
- The laser is the **armed indicator** (steady when watching, blinking during an alarm).

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first: an event log of `MOTION` lines. The
+10 extension pushes an alert to your phone (web page or a webhook).

## Ethics of this asymmetry

- PIR detects **motion, not identity** — no camera, no recording. A privacy-respecting presence sense.
- Anything that watches a shared space should be **disclosed**: tell people the tripwire is armed.
  Re-read [`../../framework/information-asymmetry.md`](../../framework/information-asymmetry.md)
  §"Ethics of Asymmetry" before pointing it at people.
- **Laser safety:** never aim the beam at eyes.

## Extension paths (climbing the ladder)

- **+10 (level 20):** Wi-Fi push — POST to a webhook or serve a status page (templates §G).
- **+10 (level 20):** add a **reed switch** on a door for "armed only when the door is shut".
- **+20 (level 30):** debounce + a real state machine (disarmed / armed / triggered) with a button.

## Credits & inspiration

Random Nerd Tutorials "ESP32 PIR interrupts" and "ESP32 Relay"; the Keyestudio PIR lesson. Every
shop-door chime and museum tripwire, in one evening.
