# Companion software — Kit Parking Sensor

The software twin. For tonight, **serial-first** is a complete, demo-able win; the web page is the
+10 stretch. See the four partner types in
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --USB serial / Wi-Fi--> [ serial monitor / phone browser ] --> [ human ]
```

## Minimal sample (serial-first — do this first)

Open the Arduino serial monitor at **115200**. You should see lines like:

```
distance: 42.6 cm
distance: 18.1 cm
```

That moving number *is* the win — it proves the sensor, the divider, and the wiring all work before
you trust the buzzer tempo.

## Extension idea (+10, level 20): a phone web page

Add `WiFi.h` + `ESPAsyncWebServer` + `AsyncTCP` (see
[`../../../build-night/templates-catalogue.md`](../../../build-night/templates-catalogue.md) §G) and
serve a one-line page that fetches the latest distance every 250 ms. Now anyone on the same Wi-Fi
watches the range from their phone — the first time the build leaves the bench.

## Extension idea (+20): let an agent narrate

Pipe the distance stream to a Claude prompt that says one honest sentence ("a car just pulled into
the bay" vs "someone walked past") — see [`../../../capabilities/reason.md`](../../../capabilities/reason.md).
