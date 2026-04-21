# Radar & LiDAR

mmWave radar, time-of-flight distance, LiDAR scanners. The "depth and presence without pixels" stack.

## Part catalog

| Part | Kind | Interface | Note |
|---|---|---|---|
| **HLK-LD2410B** | 24 GHz mmWave presence | UART | Cheap ($4), tuneable; detects human presence + motion zones. |
| **HLK-LD2420** | 24 GHz FMCW | UART | Longer range variant. |
| **HLK-LD2450** | 24 GHz, multi-target tracking | UART | Tracks up to 3 targets with position. |
| **IWR6843 / IWR1443 (TI)** | 60 / 77 GHz | UART / SPI | Pro-grade FMCW mmWave; rich point clouds. |
| **VL53L1X** | ToF distance, 1-zone | I²C | 4 m range; beam width matters. |
| **VL53L5CX** | ToF, 8×8 multi-zone | I²C | A tiny depth "camera"; gesture-level resolution. |
| **VL53L8CX** | ToF, 8×8 wider FOV | I²C | Updated L5 with better angular coverage. |
| **TF-Luna / TF-Mini Plus** | Single-beam LiDAR | UART / I²C | Up to 12 m; drone altimetry. |
| **RPLiDAR A1 / A2 / C1** | 360° 2D LiDAR | UART | Room scanning; SLAM-adjacent projects. |
| **Livox Mid / Avia** | 3D solid-state LiDAR | Ethernet | Big kid's toy; needs a host, ESP32 coordinates. |

## Interface notes

- LD2410B talks UART at 256000 baud; a library like `ld2410` simplifies parsing.
- VL53L5CX needs a firmware blob uploaded from flash; reserve ~80 KB.
- mmWave sensors dislike metal enclosures — use RF-transparent plastic (ABS, PETG).

## Capability contribution

- **Find**: presence, movement, distance, coarse pose.
- **Transform**: raw point cloud → skeletal pose (with a host).
- **Distill**: "three zones occupied, one zone still for > 20 min".
- **Reason**: radar + audio + calendar → "person working at desk, don't interrupt".
- **Represent**: heatmap of a room on TFT; ambient room-occupancy LED.

## Pairs with (asymmetry combos)

- **LD2410B + BME680 + CO₂** — "someone's been in this meeting room; the air is going bad; pop a fan on."
- **VL53L5CX + NeoPixel** — touchless gesture controller on a kitchen cabinet or dashboard.
- **mmWave + mic** — privacy-preserving baby monitor: breathing rate + crying detection, zero camera.
- **RPLiDAR + IMU** — build a cheap room-map; feed to a VLM for "what room is this?".

## Where to buy

- **AliExpress / Seeed** for HLK-LD24xx (cheap and plentiful).
- **DigiKey / Mouser** for TI IWR / ST VL53Lxx.
- **Benewake / Youyeetoo** direct for TF-Luna / TF-Mini.
- **SLAMTEC** for RPLiDAR series.

## Gotchas

- **LD2410B defaults are aggressive** — read the register tuning guide; the out-of-box sensitivity over-triggers on ceiling fans.
- **VL53L5CX cover glass** — IR-transparent plastic only; normal clear plastic blocks 940 nm.
- **RPLiDAR motor noise** is audible at low frequencies; budget for it in living-room builds.
- **Beam geometry**: the single-zone VL53L1X has a 27° cone, not a pencil. Plan mounting for beam spread.
