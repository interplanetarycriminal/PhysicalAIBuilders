# Companion software — Kit Sound-Reactive Lights

The software twin. Serial-first is the win tonight; the phone slider is the +10. See
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --USB serial / Wi-Fi--> [ serial monitor / phone slider ] --> [ human ]
```

## Minimal sample (serial-first — do this first)

Open the serial monitor at **115200** and clap. You should see the level spike:

```
sound raw:1980 level:31
sound raw:3110 level:1161     <-- clap
```

A number that jumps when you make noise *is* the first win — confirm it before chasing pretty
colours.

## Extension idea (+10, level 20): a phone sensitivity slider

Serve a one-control web page (`WiFi.h` + `ESPAsyncWebServer`, see
[`../../../build-night/templates-catalogue.md`](../../../build-night/templates-catalogue.md) §G); the
slider POSTs a threshold the firmware reads each loop. Now you tune the meter to the room without
re-flashing.

## Extension idea (+30): spectral colour

Move from raw level to an FFT (see
[`../../../capabilities/transform.md`](../../../capabilities/transform.md)) and drive hue from the
dominant frequency band — bass paints red, treble paints blue.
