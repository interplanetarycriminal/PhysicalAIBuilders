# Audio sensors

MEMS microphones, electret mics, ultrasonic, contact / piezo pickups, bone conduction. The "what does it sound like?" stack.

## Part catalog

| Part | Kind | Interface | Note |
|---|---|---|---|
| **INMP441** | I²S MEMS mic | I²S | Gold-standard hobbyist mic; low noise, 24-bit. |
| **SPH0645LM4H** | I²S MEMS mic | I²S | Adafruit's flagship mic breakout; excellent. |
| **ICS-43434** | I²S MEMS mic | I²S | TDK; good self-noise; used in premium builds. |
| **MAX9814** | Analog electret mic + AGC | Analog | Cheap, fast; use ADC with care. |
| **MSM261S4030H0R** | PDM MEMS mic | PDM → I²S | Very small; used on tiny wearable boards. |
| **Electret capsule + MAX4466** | Analog | Analog | Maker classic; OK for level, weak for speech. |
| **Bone-conduction pickup** | Contact mic | Analog | Confidentiality for throat / jaw; sub-vocal speech possible. |
| **Hydrophone + preamp** | Underwater mic | Analog | Surprising pair with ESP32 LoRa for river acoustic monitoring. |
| **HC-SR04** | Ultrasonic range | Trigger/echo | Cheap distance; up to ~4 m; poor on soft targets. |
| **MB1040 / MaxSonar** | Ultrasonic range | Analog / UART | Industrial ultrasonic; much better than HC-SR04. |
| **Piezo contact pickup** | Vibro-acoustic | Analog | Structure-borne sound — hear a machine's bearings. |

## Interface notes

- **I²S > analog** for audio on ESP32 — the analog ADC has noise from Wi-Fi. Use I²S when you can.
- ESP32 has multiple I²S peripherals; one can be input, one output simultaneously (full-duplex).
- PDM mics need a PDM-capable I²S config (available on S3, C3, C6 cleanly).
- Sample rate sweet spots: 16 kHz for speech, 44.1/48 kHz for music/ambient detail.

## Capability contribution

- **Find**: wake-word, keyword spotting, sound classification (cough, glass break, dog bark, mosquito).
- **Transform**: raw PCM → MFCC → TinyML classifier; or raw PCM → FFT → audio-reactive lighting.
- **Distill**: "three crashes, one cough, and a dog bark" as the minute's summary.
- **Reason**: audio + IMU + time of day → situation (your dog is bored, not scared).
- **Represent**: level meter, spectrogram, audio-reactive LEDs, TTS back-channel.

## Pairs with (asymmetry combos)

- **INMP441 + Edge Impulse TinyML** — classify 10 ambient sound classes on-device in ~20 KB of model.
- **Piezo pickup on a beehive** — detect swarm preparation by acoustic signature; see [`../../projects/beekeeper-hivelink/`](../../projects/beekeeper-hivelink/).
- **Ultrasonic + mmWave** — redundant presence sensors; ultrasonic ignores TV-picture motion.
- **Contact mic + ML** — predictive maintenance on a washing machine / fridge / HVAC compressor.
- **Hydrophone + LoRa** — creek / river soundscape monitoring that doesn't need Wi-Fi.

## Where to buy

- **Adafruit / SparkFun** for INMP441 / SPH0645 breakouts.
- **DigiKey / Mouser** for TDK, Knowles, Infineon MEMS bare parts.
- **MaxBotix** direct for ultrasonic.
- **Aquarian Audio** for hydrophones.
- **AliExpress / LCSC** for bulk INMP441.

## Gotchas

- **Wind noise** dominates outdoor mic signals; mount in foam or use a dead-cat windscreen.
- **Ground loops** between USB-powered ESP32 and powered speaker systems create awful hum — isolate if needed.
- **ESP32 ADC DC offset** is ~600 mV; strip it in software before FFT.
- **Privacy**: any mic build should include a hardware disable switch *and* a visible LED tied to power on the mic.
