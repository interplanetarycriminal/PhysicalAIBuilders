# Find

> Locate the needle: patterns, anomalies, objects, people, or moments of interest in noisy streams.

## In this repo's vocabulary

Find is the *search* verb. It answers "where / when / which one?" — in space, in time, in a stream of waveforms, in a mesh of nodes, in a crowd of signals.

## Classic ESP32 implementations

### On-device
- **Wake-word & keyword spotting** — INMP441 + TinyML model (~20 KB).
- **Person / face detection** — ESP32-CAM + ESP-WHO or the Useful Sensors Person Sensor (all-in-one module).
- **Anomaly detection** — autoencoder on IMU / audio / current-draw; deviation = discovery.
- **Presence finding** — LD2410B mmWave; Wi-Fi CSI for through-wall presence.
- **RF direction-finding** — ESP32 BLE RSSI triangulation across a mesh; 2.4GHz promiscuous sniff.
- **Chemical plume hunting** — MOS gas sensors + moving platform + gradient ascent.

### With companion software
- **Vector search** — ESP32 pushes spectrogram embeddings to a cloud index; queries like "find every episode of rattling-fan sound last month".
- **Cross-modal search** — audio wake ≈ "baby", camera confirms, Claude narrates what it finds.
- **Map-based find** — node mesh, geo-indexed events surfaced on a Leaflet/Mapbox dashboard.

## Sensors & add-ons that feed Find

- **Audio**: INMP441, piezo pickups, ultrasonic HC-SR04 / I2C.
- **Vision**: OV2640, OV5640, MLX90640 thermal, VL53L5CX multi-zone ToF, Useful Sensors Person Sensor.
- **Radar**: LD2410B / LD2420 mmWave, TF-Luna LiDAR.
- **RF**: Wi-Fi CSI (ESP32 native!), BLE scans, LoRa packet forensic.
- **Chemical / bio**: gas arrays, scent discrimination with TinyML.

## Failure modes

- **False positives** — Find is almost always a classifier; your precision/recall tradeoff matters more than raw accuracy.
- **Domain shift** — a model trained in your living room fails in somebody else's. Always include on-device adaptation or a "re-calibrate" button.
- **Privacy blow-back** — finding people is the most ethically loaded capability. See asymmetry ethics in [`../framework/information-asymmetry.md`](../framework/information-asymmetry.md).

## Cross-references

- Personas: **cryptid hunter**, **urban gardener (pest find)**, **wildfire lookout**, **falconer**, **parent**, **search-and-rescue diver**, **investigative journalist**.
- Flagship projects: [`projects/ghost-hunter-csi-presence/`](../projects/ghost-hunter-csi-presence/), [`projects/investigative-journalist-sourceshield/`](../projects/investigative-journalist-sourceshield/), [`projects/beekeeper-hivelink/`](../projects/beekeeper-hivelink/).
