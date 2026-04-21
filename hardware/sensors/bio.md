# Bio sensors

Heart rate, HRV, SpO2, ECG, EMG, GSR, respiration, skin temperature. The "what is the body doing?" stack.

## Part catalog

| Part | Measures | Interface | Note |
|---|---|---|---|
| **MAX30102 / MAX30105** | PPG → HR, HRV, SpO2 | I²C | Red + IR LEDs; works on fingertip or forehead. |
| **MAX30003** | Single-lead ECG | SPI | Real ECG IC, not a toy; great for HRV research. |
| **AD8232** | Single-lead ECG AFE | Analog | Classic and approachable; kit-ready. |
| **MyoWare 2.0** | EMG | Analog | Surface EMG from a muscle belly; good for intent-sensing. |
| **GSR / EDA electrodes + analog AFE** | Skin conductance | Analog | Tiny resistance changes = sympathetic arousal. |
| **MLX90614** | Non-contact IR temp | I²C | Forehead / object temperature. |
| **MAX30208** | Clinical-grade skin temp | I²C | 0.1 °C accuracy; great for circadian studies. |
| **Resp belt (piezo)** | Thoracic/abdominal expansion | Analog | Respiration rate via strain. |
| **OpenBCI / BCI electrodes** | EEG | ADS1299 SPI | Not ESP32-native but integrable via SPI expansion. |

## Interface notes

- Bio signals are tiny (microvolts for ECG/EMG/EEG). Keep wires short, shielded, and away from the ESP32 radio.
- Analog front-ends need a stable reference — 3V3 LDO dedicated to the AFE beats sharing with the radio supply.
- PPG works through thin skin (fingertip, wrist); motion kills it. Combine with IMU so you can discard motion artifacts.

## Capability contribution

- **Transform**: PPG → HR → HRV via RR-interval math.
- **Find**: arrhythmia, stress spike, apnea event.
- **Distill**: "today's stress score = 37"; "sleep efficiency = 86%".
- **Reason**: bio + calendar + caffeine log → "you're stressed because of the 3 pm meeting; tomorrow try eating lunch first".
- **Represent**: haptic heart-beat, ambient color, LLM-narrated daily journal.

## Pairs with (asymmetry combos)

- **MAX30102 + IMU** — motion-compensated HR/HRV; the "real" Fitbit recipe.
- **ECG + GSR + skin temp** — full sympathetic-arousal picture in a wristband.
- **Resp belt + mic** — sleep apnea detection (respiratory pauses + snoring).
- **EMG + IMU + BLE** — gesture control for accessibility tools; "blink-less" mouse via jaw clench.
- **Forehead PPG + thermal camera** — contactless vitals from a mantelpiece device.

## Where to buy

- **DigiKey / Mouser** for MAXIM parts.
- **SparkFun / Adafruit** for breakouts with nice silk-screened electrodes.
- **Advancer Technologies** for MyoWare.
- **BioAmp / Upside Down Labs** for hobby-grade ECG / EMG / EEG shields.

## Gotchas

- **Electrode contact** dominates signal quality; wet-gel electrodes beat dry for short sessions.
- **Medical claims** are regulated. This is a **learning** community — do not build diagnostic devices and call them medical without clearance.
- **Battery isolation** when electrodes are on a human is mandatory. Never connect electrodes to a mains-powered device without isolation.
- **HRV time-domain metrics** (RMSSD, SDNN) need at least 2–5 minutes of clean data. Budget for that in your UX.
