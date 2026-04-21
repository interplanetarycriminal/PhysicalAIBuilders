# Parkour FlowState

> **Level:** 50 — TinyML on Device. Two weeks. ~$120.
> **Tribe:** Sports & bodies (#41)
> **One-line vibe:** a wearable that learns your flow-state vs fatigue from how you move, and tells you the exact jump where you tipped over.

## The asymmetry

Athletes feel fatigue; they can't usually point to the precise moment. FlowState uses a wearable IMU + HRV and an on-device TinyML classifier to label every 2-second window as *flow, strain,* or *recovery.* Post-run, you see the exact jump where flow broke — a kind of self-knowledge ordinary video replay can't give you.

## The two-line spec

```
HARDWARE:  XIAO ESP32-S3 Sense + BMI270 IMU + MAX30102 PPG + DRV2605 haptic + 300 mAh LiPo
SOFTWARE:  Edge Impulse model (IMU+HRV → {flow, strain, recovery}) + mobile replay app
```

## Bill of materials

| Qty | Part | Role | Vendor | $ |
|---|---|---|---|---|
| 1 | Seeed XIAO ESP32-S3 Sense | MCU + PSRAM + mic (unused here) | Seeed | $14 |
| 1 | BMI270 breakout (or onboard equivalent) | 6-DOF IMU at 100 Hz | Adafruit / Pimoroni | $10 |
| 1 | MAX30102 breakout | PPG → HRV | SparkFun | $8 |
| 1 | DRV2605 + LRA | Haptic feedback | Adafruit | $10 |
| 1 | 300 mAh LiPo + protection | Power | Adafruit | $8 |
| 1 | Wristband 3D print + fabric | Form factor | — | — |

**Total:** ~$60 + labeling time.

## Wiring

```
XIAO ESP32-S3 Sense (I²C on D4/D5 by default)
 ├─ BMI270 SDA/SCL     ─ shared I²C (pull-ups on breakout)
 ├─ MAX30102 SDA/SCL   ─ shared I²C
 ├─ DRV2605 SDA/SCL    ─ shared I²C
 └─ LiPo via onboard BQ25628 charger + slide switch
```

All three sensors on one I²C bus, each with unique address — SDA/SCL + 3V3 + GND only.

## Firmware

See [`firmware/main.cpp`](firmware/main.cpp). Arduino-ESP32 via PlatformIO; libs: `SparkFun_BMI270_Arduino`, `MAX30105` Sparkfun fork, `Adafruit_DRV2605`, and the **TFLite Micro** runtime with an exported Edge Impulse model.

Pipeline (runs at 100 Hz IMU, 50 Hz PPG):

1. Windowed feature extraction: 2 s, 50% overlap.
2. Features: IMU mean/std per axis + spectral power bands, HRV metrics (RR-interval-based), PPG variability.
3. Model: 1D-CNN Edge-Impulse export; 3-class softmax; ~30 KB.
4. If class transitions from `flow → strain` → 2-pulse haptic pattern.

## Companion software

See [`companion/README.md`](companion/README.md). A mobile web PWA that:

1. Connects over BLE and streams the 2 s labels + raw features.
2. Records a session with GPS (phone).
3. Shows a timeline post-run; tap a jump to see the label history.
4. Optional LLM session review.

## Ethics of this asymmetry

Personal, opt-in data. When sharing with a coach, export a *redacted* CSV (labels only, no raw).

## Extension paths (climbing the ladder)

- **+10 (level 60):** mesh two wristbands (ankle + wrist) over ESP-NOW for full-body labels.
- **+20 (level 70):** shrink everything onto a custom PCB with a co-processor for on-device video snippets at break-points.
- **+30 (level 80):** add a Claude session review — describe the break-points in natural language.

## Data labeling workflow

Labeling is where this build really lives. You wear the band for 30 sessions and label each session post-hoc ("the first 10 minutes were flow; after the back-wall it was all strain"). Edge Impulse ingests labeled windows; model quality scales with your labeling discipline.

## Credits & inspiration

Edge Impulse tutorials, BMI270 low-power examples, HRV research on state detection.
