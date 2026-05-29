# Companion software — Kit RFID Doorman

The software twin. Serial-first to capture UIDs; a server-side allowlist is the +10. See
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --serial / Wi-Fi--> [ monitor / allowlist server ] --> [ human or agent ]
```

## Minimal sample (serial-first — do this first)

Open the serial monitor at **115200** and tap each card:

```
doorman ready - tap a card.
UID: 04 A3 19 2B
DENIED
```

Copy that `UID` into the `known[]` array in the firmware, re-flash, tap again → `ACCESS GRANTED` and
the servo opens. Capturing a UID *is* the first win.

## Extension idea (+10, level 30): a server-side allowlist

Instead of hard-coding `known[]`, have the ESP32 ask a tiny server (Flask / FastAPI) "is this UID
allowed?" over Wi-Fi (templates [§G](../../../build-night/templates-catalogue.md)). Now you grant and
revoke access from a phone without re-flashing.

## Extension idea (+20): an agent concierge

Log `UID + timestamp` and let a Claude tool decide access by schedule and greet known holders by name
(see [`../../../capabilities/reason.md`](../../../capabilities/reason.md)). Keep the log local and
short-lived — see the ethics note in the project README.
