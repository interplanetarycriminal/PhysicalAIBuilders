# Companion software — Kit Tripwire Alert

The software twin. Serial-first tonight; phone push is the +10. See
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --serial / Wi-Fi webhook--> [ event log / phone notification ] --> [ human ]
```

## Minimal sample (serial-first — do this first)

Open the serial monitor at **115200**, arm the device, and wave a hand past the PIR:

```
armed.
MOTION - tripwire crossed!
```

A line that appears exactly when you cross the beam *is* the win — confirm it before wiring the relay
to a load.

## Extension idea (+10, level 20): push to your phone

Add Wi-Fi and POST to a webhook (IFTTT / ntfy / a tiny Flask endpoint) on each trip, or serve a
status page (`ESPAsyncWebServer`, see
[`../../../build-night/templates-catalogue.md`](../../../build-night/templates-catalogue.md) §G). Now
the doorway alerts you from across the building.

## Extension idea (+20): an agent decides

Feed the event timeline to a Claude prompt that distinguishes "expected" from "unexpected" entries by
time of day (see [`../../../capabilities/reason.md`](../../../capabilities/reason.md)) — fewer false
alarms, one honest sentence.
