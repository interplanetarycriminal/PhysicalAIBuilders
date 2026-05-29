# Companion software — Kit Weather Station

The software twin. Serial-first tonight; a phone web page is the +10. See
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --serial / Wi-Fi--> [ serial monitor / LCD / phone page ] --> [ human ]
```

## Minimal sample (serial-first — do this first)

Open the serial monitor at **115200**:

```
temp:23.4C  humidity:51%
```

A plausible temperature and humidity *is* the win — confirm it before fighting the LCD library.

## Extension idea (+10, level 20): a phone web page

Add `WiFi.h` + `ESPAsyncWebServer` + `AsyncTCP` (templates
[§G](../../../build-night/templates-catalogue.md)) and serve a page that shows live T/RH and
auto-refreshes. Now you read the room from your phone.

## Extension idea (+20): an LLM weather line

POST the readings to a Claude prompt that writes one plain sentence ("muggy — open a window")
instead of raw numbers — see [`../../../capabilities/reason.md`](../../../capabilities/reason.md).
