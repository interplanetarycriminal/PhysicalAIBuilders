# Transform

> Reshape data from one representation into another. Raw accel into gesture. Audio into transcript. Image into depth. Pressure into altitude.

## In this repo's vocabulary

Transform is the *change-of-basis* verb. The information content is roughly preserved but it is made useful by translating it into a representation the next stage (human, model, actuator) can consume directly.

## Classic ESP32 implementations

### On-device
- **FFT** — microphone → frequency bands, for audio-reactive lighting, or rotor imbalance detection.
- **IMU fusion** — 6/9-DOF → Madgwick/Mahony → orientation quaternion.
- **Gesture classification** — IMU → TinyML model (Edge Impulse, TFLite Micro) → "left swipe / punch / idle".
- **Thermal remapping** — MLX90640 24×32 grid → interpolated 120×160 false-color bitmap on TFT.
- **CSI → feature vector** — ESP32 Wi-Fi channel-state-information raw → filtered gait/presence features.
- **Pressure → altitude** — BMP388 → meters above sea level for paraglide/altimeter builds.
- **VOC + humidity → IAQ score** — BME680 internal state machine already does this.

### With companion software
- **Audio → transcript** — ESP32 streams PCM → Whisper on Pi/laptop → captions.
- **Image → depth** — ESP32-CAM image → cloud MiDaS/Depth-Anything → depth map streamed back.
- **Raw telemetry → embeddings** — sensor window → transformer embedding → vector DB for "what is similar to now?"

## Sensors & add-ons that feed Transform

Effectively all of them. Transform sits between a raw sensor and every higher-level verb.

## Failure modes

- **Numerical instability** in on-device FFT — always window (Hann), always normalize.
- **Sensor bias drift** — especially IMU gyros; re-bias during stillness windows.
- **Model quantization loss** — TinyML INT8 can degrade odd-shaped distributions; evaluate on device, not just on PC.
- **Confusion of transform with find** — a classifier that outputs "probabilities" is doing Find, not Transform. If the output's dimensionality drops dramatically, you've slid from Transform into Distill.

## Cross-references

- Personas: **parkour athlete**, **boxing cornerman**, **avalanche forecaster**, **DJ**, **paramedic**.
- Flagship project: [`projects/parkour-flowstate/`](../projects/parkour-flowstate/).
