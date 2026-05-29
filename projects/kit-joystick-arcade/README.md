# Kit Joystick Arcade

> **Level:** 20 — Connected Object. One session. Kit-only.
> **Tribe:** Arts & performance
> **One-line vibe:** steer a glowing dot around an 8×8 grid — your first hand-built game.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. A
> [build-night](../../build-night/) crowd-pleaser. All parts are 3.3V-safe; on the **S3** remap I²C
> (no GPIO22). See [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## The asymmetry

This one's playful rather than world-reading — but it teaches the core loop of every interactive
device: read a continuous human input, map it to state, render the state, repeat at speed. Master
this and you can build any controller, instrument, or UI. The "asymmetry" is craft: you now know how
the games you grew up with actually work.

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + Joystick (2 analog + button) + HT16K33 8×8 dot matrix
SOFTWARE:  on-device input→cursor mapping + matrix render; serial print of x/y/button
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | Joystick Module | 2-axis analog + push | Keyestudio 42-in-1 kit | in kit |
| 1 | HT16K33 8×8 Dot Matrix (I²C) | Display | Keyestudio 42-in-1 kit | in kit |

**Total:** ~$0 beyond the kit.

## Wiring

```
Joystick                    ESP32 (classic)
 VRx ──► GPIO34             (ADC1, input-only)
 VRy ──► GPIO35             (ADC1, input-only)
 SW  ──► GPIO32             (button, INPUT_PULLUP)
 V   ──► 3V3   G ──► GND

HT16K33 8×8 (I²C, addr 0x70)
 SDA ──► GPIO21             (S3: Wire.begin(8, 9))
 SCL ──► GPIO22             (⚠ no GPIO22 on the S3)
 V   ──► 3V3   G ──► GND
```

> Both joystick axes go to **ADC1** pins (32–39) so they keep reading once Wi-Fi is on.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework. Libraries: `Adafruit LED Backpack` +
`Adafruit GFX`.

Key choices:

- A **deadzone** around centre stops the cursor drifting when the stick is released.
- The cursor is clamped to the 0–7 grid; the button press flashes a frame (a hook for "fire"/"drop").
- Serial print of x/y/button is the first win and the debugging surface.

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first; the +10 turns the ESP32 into a BLE
gamepad (S3) or a Wi-Fi-controlled display.

## Ethics of this asymmetry

- A self-contained toy: no sensing of other people, no data leaves the device. Nothing to disclose.

## Extension paths (climbing the ladder)

- **+10 (level 30):** add a target pixel and score — a real "catch the dot" game.
- **+10 (level 20):** drive the matrix from a phone over Wi-Fi instead of the joystick.
- **+20 (level 40):** two boards over **ESP-NOW**, each a paddle — local two-player Pong.

## Credits & inspiration

Adafruit LED Backpack guide; the Keyestudio joystick lesson. Every handheld you ever wore out, in
miniature.
