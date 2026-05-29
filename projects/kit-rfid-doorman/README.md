# Kit RFID Doorman

> **Level:** 20 — Connected Object. One session. Kit-only.
> **Tribe:** Spies & Shadows / High-stakes operations
> **One-line vibe:** tap the right card and a servo "door" swings open with a friendly beep.
>
> 🧰 **Built tonight with the May 29 kit.** No parts beyond the Keyestudio 42-in-1 kit. A
> [build-night](../../build-night/) crowd-pleaser. ⚠ The **RC522 reader is 3.3V only — never power it
> at 5V.** The servo is a **5V** part: power it from the expansion board's 5V rail. See
> [`../../build-night/safety-and-gpio.md`](../../build-night/safety-and-gpio.md).

## The asymmetry

A lock knows one secret: the right key. This build generalises that — the wielder decides, in
software, *which* tokens are trusted, and can change the rule instantly without re-cutting a key.
Knowing a card's UID (which the holder can't see) and acting on it physically is the seed of every
access-control system.

## The two-line spec

```
HARDWARE:  ESP32-WROOM-32E (classic) + RC522 RFID reader (3.3V) + 9G Servo (5V) + Active Buzzer
SOFTWARE:  on-device UID match → open/deny; serial print of every UID; optional cloud allowlist
```

## Bill of materials

| Qty | Part | Role | Vendor(s) | Approx $ |
|---|---|---|---|---|
| 1 | ESP32-WROOM-32E (or Keyestudio Plus) | MCU | Keyestudio 42-in-1 kit (KS5003) | in kit |
| 1 | RFID Reader Module (RC522) | Read card UID (3.3V) | Keyestudio 42-in-1 kit | in kit |
| 1 | White Card + RFID Key Fob | The tokens | Keyestudio 42-in-1 kit | in kit |
| 1 | 9G 90° Servo | The "door" (5V) | Keyestudio 42-in-1 kit | in kit |
| 1 | Active Buzzer | Grant/deny feedback | Keyestudio 42-in-1 kit | in kit |
| 1 | ESP32-IO Expansion Board + 6×AA holder | 5V rail for the servo | Keyestudio 42-in-1 kit | in kit |

**Total:** ~$0 beyond the kit.

## Wiring

```
RC522 (SPI, 3.3V ONLY)      ESP32 (classic)
 SDA/SS ──► GPIO21
 SCK    ──► GPIO18
 MOSI   ──► GPIO23
 MISO   ──► GPIO19
 RST    ──► GPIO22          (S3: pick a valid pin — no GPIO22 on the S3)
 3.3V   ──► 3V3             ⚠ never 5V
 GND    ──► GND

9G Servo
 SIG ──► GPIO13
 V   ──► 5V (expansion/AA rail)        ⚠ not the 3.3V pin
 G   ──► GND (common ground)

Active Buzzer
 SIG ──► GPIO4   V ──► 3V3   G ──► GND
```

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino framework. Libraries: `MFRC522` (SPI) +
`ESP32Servo`.

Key choices:

- Every scanned UID is **printed to serial** — that's how you discover your own card's UID to paste
  into the `known[]` allowlist (do this first).
- A match sweeps the servo to "open", holds, then closes; a non-match gives one long deny beep.
- The buzzer is the kit **Active Buzzer** (on/off), so feedback is `digitalWrite` pulses, not tones.

## Companion software

See [`companion/README.md`](companion/README.md). Serial-first to capture UIDs; the +10 moves the
allowlist to a tiny server so you can grant/revoke access without re-flashing.

## Ethics of this asymmetry

- A card UID is a **token, not an identity** — but an access log *is* a record of who went where and
  when. Keep it local, keep retention short, and tell people the door logs taps. See
  [`../../framework/information-asymmetry.md`](../../framework/information-asymmetry.md) §"Ethics of Asymmetry".
- RC522 UIDs are trivially cloneable — this is a **demo**, not a security product. Don't guard
  anything that matters with it.

## Extension paths (climbing the ladder)

- **+10 (level 30):** move the allowlist to a server; grant/revoke from a phone (templates §G).
- **+10 (level 20):** add the **DS1307 RTC** and log `UID + timestamp`.
- **+20 (level 40):** an LLM concierge that greets known cards by their assigned nickname on the LCD.

## Credits & inspiration

MFRC522 library examples; the Keyestudio RFID lesson. The office badge reader, rebuilt so you can see
exactly how it decides.
