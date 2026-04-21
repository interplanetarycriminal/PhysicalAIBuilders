# Pirate Stage Walk-By

> **Level:** 30 — Opinionated Firmware (creative track). A weekend. ~$40.
> **Tribe:** Creative remix (#101, Stage Walk-By Magician)
> **One-line vibe:** walk past a hallway camera → appear on a projected wall as a pirate from the waist up, lower half replaced by a rocking boat hull on procedural seas. The faster you walk, the rougher the waves.

> **This project exists to prove a principle**, not to monitor a hallway. Read [`../../framework/software-as-imagination.md`](../../framework/software-as-imagination.md) first — the ESP32 here does almost nothing interesting; the **software twin** decides the signal means "pirate on rough seas."

## The asymmetry

Most hallway cameras either (a) watch nothing, or (b) report boring facts about occupancy. This build takes the same raw signal ("person passed; direction; speed") and **re-assigns meaning**: the signal becomes a theatrical event. Strangers grin. Kids run back and forth to check it. A cat becomes a sea-monster cameo.

Mode (from the twelve): **Environmental + World + RoleReversal**.

## The two-line spec

```
HARDWARE:  ESP32-CAM (or ESP32-S3 + OV2640) pointed down a hallway; a laptop + projector displaying a browser scene
SOFTWARE:  ESP32 runs light person-detect + publishes {seen, x, v, direction}; a p5.js scene in the browser renders the pirate-and-sea world
```

The ESP32 is disposable. The software is the star.

## Bill of materials

| Qty | Part | Role | Vendors | Approx $ |
|---|---|---|---|---|
| 1 | AI-Thinker ESP32-CAM | Camera + Wi-Fi | Adafruit, AliExpress | $8 |
| 1 | FTDI / USB-serial adapter | Programming | anywhere | $5 |
| 1 | 5 V 2 A PSU | Power | anywhere | $6 |
| 1 | Laptop (any) running Chrome / Firefox | Renders the scene | you already have one | — |
| 1 | Short-throw projector OR a big monitor | Display | hackerspace share / school / home | — |
| 1 | 3D-printed camera mount | Above-door mounting | print local | — |

**Total:** ~$20 + stuff you already own.

## Wiring

Nothing unusual. ESP32-CAM flashed via FTDI; 5V power only. Camera mounted above the hallway pointing straight down or at a shallow angle along the walk-line.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino IDE + ESP32-CAM board package.

The firmware is deliberately minimal:

- Captures frames at ~5 fps.
- Computes a simple background-subtraction silhouette (no ML needed for this effect).
- Every frame, publishes `{seen: bool, x_norm: 0..1, v_norm: -1..1}` over WebSocket at ws://<ip>:81/.
- **That's it.** All creativity lives in the browser.

If you want heavier vision, swap the silhouette code for ESP-WHO person detection or a Grove Vision AI V2 + bounding-box forwarder — the WebSocket schema stays the same.

## Companion software

See [`companion/README.md`](companion/README.md) for the full browser scene. The pattern:

```
[ESP32 camera] --ws--> [browser p5.js scene] --projects--> [wall]
                                   ↑
                           (imagination lives here)
```

The scene:

1. Maintains a list of "pirate sprites" — one per currently-visible silhouette.
2. Each pirate is drawn as a cached upper-body SVG illustration; you can swap the sprite easily (pirate → astronaut → ghost → 1920s flapper).
3. Below each sprite, a procedural ocean: `sin`-waves whose amplitude is the person's speed and whose phase rolls in the direction they're walking.
4. Couples who pass within 800 ms become a shared galleon; three people become a crew; more become a "parade."
5. Occasional seagulls drift by (generative embellishment).
6. Audio: a faint ocean layer; footsteps trigger a gentle creak-of-timbers sample.

## Ethics of this asymmetry

- **Visible consent.** A small plaque next to the camera describes what the system does. People who don't want to be rendered can pass on the other side of the corridor.
- **No recording.** Silhouettes are computed frame-by-frame and discarded; no pixels leave the ESP32.
- **No identification.** The scene renders *everyone* as pirates; it does not look up who you are.
- **Children.** If installed in a kid-facing context (school, museum), pre-register with the adults.

## Extension paths (creative, not "climbing the ladder")

This is a creative project — the "next level" isn't harder hardware, it's bigger imagination.

- **Swap the world.** Space station. Underwater coral reef. Medieval siege. Victorian ballroom. One line of CSS / one sprite swap.
- **Time of day shifts the world.** Morning → sunrise sails; afternoon → doldrums; evening → storm.
- **Add a narrator.** A one-sentence LLM-generated caption appears as each pirate passes ("Captain Bess, finally back from the butcher").
- **Multi-wall.** Two cameras in two corridors; people crossing between them "sail" from one sea to another on a shared map.
- **Machine-theatre variant.** Two projected walls, no people — the system invents passers-by for itself.
- **A weekly archive.** The system takes a single-frame silhouette of each unique passer-by and paints a "ship's log" mural of the week.

## Why we put this at Level 30

- Firmware itself is easy; the interesting code is Javascript.
- PlatformIO discipline + WebSocket streaming is exactly the Level-30 skill unlock.
- Forces the builder to think **software twin first**, which is the main thesis of [`../../framework/software-as-imagination.md`](../../framework/software-as-imagination.md).

## Credits & inspiration

- Camille Utterback & Romy Achituv's *Text Rain* (1999), Rafael Lozano-Hemmer's *Body Movies*, every MIT Media Lab lobby.
- The Processing / p5.js teaching community.
- The endless sea-shanty renaissance.
