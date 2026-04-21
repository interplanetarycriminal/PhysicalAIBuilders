# Connectivity

Wi-Fi, Bluetooth, BLE, ESP-NOW, Thread, Zigbee, Matter, LoRa, LoRaWAN, nRF24, cellular.

## What's built in

| Variant | Wi-Fi | BT Classic | BLE | 802.15.4 | Notes |
|---|---|---|---|---|---|
| ESP32 | Wi-Fi 4 | Yes | BLE 4.2 | No | The veteran. |
| ESP32-S2 | Wi-Fi 4 | — | — | No | USB-native, single radio. |
| ESP32-S3 | Wi-Fi 4 | — | BLE 5 | No | Best for AI/ML. |
| ESP32-C3 | Wi-Fi 4 | — | BLE 5 | No | Cheap + low power. |
| ESP32-C6 | Wi-Fi 6 | — | BLE 5 | **Yes (Thread/Zigbee)** | Matter-ready. |
| ESP32-H2 | — | — | BLE 5 | Yes | Thread/Zigbee only. |
| ESP32-P4 | — | — | — | — | Compute-only; pair with C6 for radios. |

## Protocol choices

### Wi-Fi
- Great bandwidth, heavy power, existing home router works.
- `esp_wifi` API + Wi-Fi CSI expose low-level features.

### BLE
- Great for phones, wearables, beacons.
- Central / peripheral / observer roles all supported. BLE 5 long range on C3/S3/C6.

### ESP-NOW
- Espressif's **connectionless** broadcast between ESP32s. No router needed.
- ~1 ms latency; reach ~200 m line-of-sight. Great for small mesh and live performance.

### Thread / Matter
- C6 / H2 give you Thread; Matter stack atop either Wi-Fi or Thread.
- Enables real interop with Apple / Google / Amazon smart-home ecosystems.

### LoRa (SX126x + ESP32 + companion chip)
- Kilometer range, tens-of-bytes packets, ISM band.
- Meshtastic is the community's off-grid messaging layer.

### nRF24L01+
- Cheap, fast (2 Mbps), tiny range — classic DIY mesh fabric.

### Cellular
- Walter / SIM7080G / SIM7672 modules add LTE-M / NB-IoT / 4G.
- Good for field deployments with no Wi-Fi.
- Power budget is real: cellular TX eats ~500 mA peaks.

## Capability contribution

Connectivity is the transport for every other verb; pick based on range, latency, power, bandwidth:

| Trade-off | Pick |
|---|---|
| <10 m, lots of data | Wi-Fi |
| <10 m, phones, ambient | BLE |
| <100 m, fast, peer-to-peer, low overhead | ESP-NOW |
| <100 m, smart-home ecosystem interop | Thread + Matter |
| ~1 km, tiny packets, off-grid | LoRa |
| Cellular coverage needed | LTE-M / NB-IoT |

## Pairs with (asymmetry combos)

- **ESP-NOW mesh + IMU swarm** — sync 12 body-IMUs as a motion-capture rig.
- **BLE + e-paper badge** — party ice-breaker where your badge says who's nearby.
- **LoRa + GPS + IMU** — falconry / livestock / backcountry tracker.
- **Matter + mmWave + CO₂** — a room sensor your partner, roommate, or smart-home agent can act on.
- **LTE-M + PM2.5** — wildfire / air-quality citizen-sensor in places with no Wi-Fi.

## Where to buy

- **Heltec / LILYGO / RAK** for ESP32+LoRa combo boards.
- **Seeed / Sequans (Walter)** for ESP32-S3 + modem boards.
- **Olimex / DigiKey** for cellular modules.

## Gotchas

- **Regulatory bands** — LoRa 868 MHz (EU) vs 915 MHz (US). Always buy the right variant.
- **Wi-Fi + BLE coexistence** on single-core ESP32 is fine but adds latency; if timing matters, put radio on one core, app on the other.
- **ESP-NOW + Wi-Fi STA** requires matching channel; plan for it.
- **Cellular data plans** — check MVNO pricing before you commit an enclosure; a $5/month plan vs $50/month changes project economics.
