# Falconer WingComm

> **Level:** 60 — Mesh & Swarm. 2–4 weeks. ~$200.
> **Tribe:** Nature & survival (#23)
> **One-line vibe:** a pair of devices — one on the falcon's jesses, one on your glove — that link your bird's flight data back to a quiet little display on your wrist.

## The asymmetry

Traditional telemetry gives you a GPS point. WingComm gives you *context*: altitude, vertical speed, heading, "has the bird stooped", paired with a 2-D map on the glove. You always know where the bird is in space, and you know it without looking down at a phone.

## The two-line spec

```
HARDWARE:  bird: LILYGO T-Beam (ESP32+LoRa+GPS) + BMP390 + ICM-20948 + tiny LiPo
           glove: LILYGO T-Display-S3 + LoRa radio + haptic
SOFTWARE:  LoRa link (bird→glove) + phone bridge app (map + logs) + local LLM post-flight
```

## Bill of materials

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 1 | LILYGO T-Beam v1.1 (868/915 MHz) | bird MCU + LoRa + GPS + holder | LILYGO | $45 |
| 1 | BMP390 breakout | altitude | Adafruit | $12 |
| 1 | ICM-20948 | 9-DOF | SparkFun | $17 |
| 1 | 350 mAh LiPo | bird power | Adafruit | $8 |
| 1 | LILYGO T-Display-S3 + LoRa shield | glove console | LILYGO | $35 |
| 1 | DRV2605 + LRA | glove haptic | Adafruit | $10 |
| 1 | 18650 holder | glove battery | — | $5 |
| 1 | Custom 3D-printed bird enclosure | aerodynamic housing | design / print | — |

**Total:** ~$140.

## Wiring

**Bird side**

```
T-Beam (ESP32 + NEO-M8N GPS + SX1276 LoRa already wired)
 └─ I²C expansion ── BMP390, ICM-20948
```

**Glove side**

```
T-Display-S3 + LoRa (integrated)
 └─ I²C ── DRV2605
```

## Firmware

See [`firmware/bird_main.cpp`](firmware/bird_main.cpp) and [`firmware/glove_main.cpp`](firmware/glove_main.cpp). PlatformIO; libs: `RadioLib`, `TinyGPS++`, `Adafruit_BMP3XX`, `SparkFun_ICM-20948`, `TFT_eSPI`, `Adafruit_DRV2605`.

Bird side:

- 5 Hz LoRa broadcast packet: `{seq, lat, lon, alt_m, vspeed_mps, heading_deg, g_peak}`.
- Deep-sleep aggressively when grounded (vibration threshold on IMU).
- Stoop detection: vertical speed < −15 m/s for 500 ms → `g_peak` flag set.

Glove side:

- Receives packets, draws a small moving-dot map with the glove at the center, bird as a dot.
- Pulses haptic on stoop or on packet loss > 2 s (the ritual "where is it?" cue).
- Stores every session to SD for phone review.

## Companion software

See [`companion/README.md`](companion/README.md). A phone PWA that pairs with the glove over Wi-Fi (glove SoftAP after landing) and pulls the session track + summary. An LLM post-flight log.

## Ethics of this asymmetry

- **Welfare first.** The bird wears at most 3% of its body weight; test enclosure weight against the bird's species guidelines before flying.
- No video on the bird — the community here is about telemetry + respect for the practice, not spectacle.
- Respect local airspace regulations around LoRa power and antennas.

## Extension paths (climbing the ladder)

- **+10 (level 70):** custom PCB for the bird enclosure; solar assist on the glove; dragonfly-shaped 3D-printed shell.
- **+20 (level 80):** local LLM post-flight analyzer that suggests the next training.
- **+30 (level 90):** cross-falconer LoRa mesh (with consent) for shared airspace awareness at a meet.

## Credits & inspiration

Modern falconry telemetry vendors (Marshall, Tracking Solutions) whose prices we don't have to match. LoRa and Meshtastic culture for the low-bandwidth mindset.
