# Companion software — Kit Joystick Arcade

The software twin. The game runs fully on-device; the companion is the stretch. See
[`../../../framework/software-hardware-pairing.md`](../../../framework/software-hardware-pairing.md).

## Architecture

```
[ ESP32 ] --serial / BLE HID / Wi-Fi--> [ monitor / PC game / phone ] --> [ human ]
```

## Minimal sample (serial-first — do this first)

Open the serial monitor at **115200** and wiggle the stick:

```
x:3 y:3 btn:0
x:4 y:3 btn:0
x:4 y:2 btn:1     <-- pressed
```

Numbers that track the stick (and a button that flips to 1) *are* the win — confirm before debugging
the matrix.

## Extension idea (+10): BLE gamepad on the S3

Flash an S3 from the pool and present a **BLE HID** gamepad (`BLEDevice.h`); now the joystick drives a
real game on a PC or phone — and you're on the doorstep of the
[`anthropics/claude-desktop-buddy`](../../../build-night/README.md) "agent reacts physically" pattern.

## Extension idea (+20): two-player over ESP-NOW

Two boards, each a paddle, talking ESP-NOW — local Pong with no router (see
[`../../../capabilities/do.md`](../../../capabilities/do.md)).
