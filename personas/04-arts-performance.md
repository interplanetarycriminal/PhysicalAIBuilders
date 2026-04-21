# Arts & performance

Ten personas for whom the ESP32 is an instrument.

---

## 31. The DJ — BPM sympath
**Asymmetry.** Live BPM of the room's dancers vs. the BPM of the track → decide to drop or hold.
**Build.** ESP32-S3 + mic + mmWave motion-count + DMX out. Find/Transform/Do.
**Companion software.** A VJ app that accepts the BPM delta and auto-selects visual intensity. See [`../projects/dj-bpm-sympath/`](../projects/dj-bpm-sympath/).
**Why it's fun.** An instrument only you hear that makes your set respond to the crowd.

## 32. The Light Artist — ambient generative wall
**Asymmetry.** A wall that breathes with the air it's in.
**Build.** ESP32 + SGP41 + mic + 300× NeoPixel. Transform/Generate/Do/Represent.
**Companion software.** Procedural shader in MicroPython; a "weather of the gallery" daily log.
**Why it's fun.** It's the gallery itself — the ESP32 makes the building the artwork.

## 33. The Choreographer — full-body motion capture
**Asymmetry.** Record a dance in full-body IMU data, not video.
**Build.** 7× BMI270 + XIAO ESP32-C3 per pod + ESP-NOW sync to a master. Transform/Represent.
**Companion software.** A web replay that shows a skeleton + a VLM that watches and gives notes.
**Why it's fun.** Rehearsal notes that don't require rewatching video.

## 34. The Mime — haptic-only echo
**Asymmetry.** Silent language. Cue lights via a wearable that the mime feels but nobody hears.
**Build.** Wristband: XIAO ESP32-C3 + DRV2605 + LRA. Mic in hat → ESP-NOW trigger. Find/Do.
**Companion software.** A phone app where a partner sends haptic cues from the audience.
**Why it's fun.** Art + accessibility demo all in one.

## 35. The Street Magician — palmed RFID
**Asymmetry.** Knows which "random" card the volunteer took.
**Build.** Palm-mounted PN532 NFC reader + haptic buzzer + concealed ESP32-C3. Find/Do.
**Companion software.** Pre-printed RFID-tagged "cards"; a minimal phone app that shows the read ID to the magician's partner.
**Why it's fun.** Classic magic gets a polite modern upgrade.

## 36. The Stage Manager — cue + audience console
**Asymmetry.** Knows audience attention (CO₂ rising means crowd is bored + warm) in real time.
**Build.** ESP32-S3 + mmWave + CO₂ + mic + DMX/OSC bridge. Distill/Do.
**Companion software.** QLab / ETC console integration + a quiet "crowd-pulse" sidebar.
**Why it's fun.** A technical director's dream.

## 37. The Installation Artist — data-breathing sculpture
**Asymmetry.** Public art that makes invisible data physical (traffic noise → light pulses).
**Build.** ESP32 mesh + PM2.5 + mic + stepper servos. Transform/Do.
**Companion software.** A plaque you can point a phone at (BLE beacon) that explains what the sculpture is reflecting today.
**Why it's fun.** Art that doesn't need a guide to explain.

## 38. The Haiku Poet — morning-weather e-paper
**Asymmetry.** A poem a day, seeded from your house's actual weather.
**Build.** LILYGO T5 e-paper + BME680 + SGP41. Transform/Generate/Represent.
**Companion software.** An LLM that takes a JSON of overnight sensor extrema and writes a 5-7-5 in your voice.
**Why it's fun.** Daily tiny joy. A beautiful gift.

## 39. The Kinetic Sculptor — articulated art
**Asymmetry.** Every joint of the sculpture knows its angle to 0.1°.
**Build.** ESP32-S3 + BLDC motors + AS5600 encoders per joint + SimpleFOC. Do/Represent.
**Companion software.** Blender-like keyframe editor that streams to the sculpture over Wi-Fi.
**Why it's fun.** Real craft; beautiful motion; deep soldering reward.

## 40. The VJ — audio-reactive visuals + DMX
**Asymmetry.** Live FFT-driven visuals + stage lights reacting together.
**Build.** ESP32-S3 + INMP441 + DMX out + HDMI-out companion (Pi). Transform/Do.
**Companion software.** OPENRNDR or Hydra sketches receiving FFT bands over UDP.
**Why it's fun.** The easiest way to jump from bedroom coder to actual stage artist.
