# Spies & Shadows

Ten personas who trade in knowing what others don't. For each: the asymmetry their ESP32 build gives them, and the software twin that turns signal into story.

---

## 1. The Corporate 007 — counter-bug sweep
**Archetype.** Boardroom-grade 007; not the tuxedo, the earpiece.
**Asymmetry.** Knows which conference rooms are bugged — before the meeting starts.
**Build.** ESP32 (CSI-capable) + RTL-SDR via companion Pi + ultrasonic mic + MLX90640 thermal + LD2410B. Find/Transform/Reason.
**Companion software.** Phone app that does a 30-second "sweep" — overlays thermal hot-spots (idle bug transmitters run warm), RF emitters by band, and ultrasonic anomalies, then asks Claude to narrate the sweep.
**Why it's fun.** The least theatrical, most impressive spy gadget in the room. Your boss will ask what it is.

## 2. Spy Kids (Juni) — kid-friendly mission kit
**Archetype.** 10-year-old with a smart backpack.
**Asymmetry.** Knows when a "locked" cookie jar has been opened since school started.
**Build.** XIAO ESP32-S3 + magnetic door sensor + IMU + INMP441 + piezo buzzer + e-paper mission brief. Find/Represent.
**Companion software.** A kid-simple phone app ("Mission Control") that sends missions: "Operative: was the cookie jar opened? Report back!" and lets the kid record & upload an audio answer.
**Why it's fun.** A gateway-drug project for a child. Cheap, safe, and produces a shareable mission log.

## 3. Ethan Hunt — impossible-mission recon badge
**Archetype.** Field operative needing 30-second situational awareness.
**Asymmetry.** Knows room occupancy + who's armed (metal/weight) + exits before entering.
**Build.** ESP32-S3 + LD2450 (multi-target mmWave) + magnetometer array + BLE sniff. Find/Reason/Represent.
**Companion software.** Phone-mounted tactical overlay that hears the badge's ESP-NOW broadcast and paints room occupancy zones on the home screen.
**Why it's fun.** Cosplay-ready, but genuinely useful for security professionals.

## 4. Jason Bourne — tail-detector
**Archetype.** Anonymous person who can't afford to be followed.
**Asymmetry.** Knows which BLE / Wi-Fi MAC addresses have been near them repeatedly across the last hour, across locations.
**Build.** ESP32 + companion Android app. Find/Distill/Reason. Uses BLE passive scan + Wi-Fi probe-request capture on the ESP32 side.
**Companion software.** Phone app with a privacy-forward design: hashes MACs locally, keeps a rolling 24-hour window, alerts if any hash has reappeared across ≥3 distinct locations.
**Why it's fun.** A rare tool where the *data erasure* is the point. Strong material for a "privacy awareness" workshop.

## 5. Harriet the Spy — e-paper notebook
**Archetype.** Curious kid / journalist-in-training.
**Asymmetry.** Carries a stealth notebook that silently logs timed observations.
**Build.** Lilygo T5 e-paper + ESP32 + a single haptic button. Do/Represent/Distill.
**Companion software.** A Claude-backed "ghostwriter" that turns terse button-timed notes plus audio snippets into a daily Harriet-voiced journal.
**Why it's fun.** Best gift to a curious child who already writes in their head.

## 6. Carmen Sandiego — multi-city passport
**Archetype.** Globe-trotter solving puzzles by inhabiting cities briefly.
**Asymmetry.** Knows the "fingerprint" of a city — Wi-Fi SSIDs, BLE beacons, language snippets in the air — and identifies the city in under 60 seconds.
**Build.** XIAO ESP32-S3 + mic + BLE + Wi-Fi scan + GPS. Find/Transform/Distill.
**Companion software.** Cloud index of 100-plus city fingerprints trained from community contributions; phone app shows the guess + confidence + "here's what gives it away".
**Why it's fun.** Frictionless travel souvenir. Make it a global community dataset.

## 7. Sherlock Holmes — trace-evidence micro-lab
**Archetype.** Consulting detective who reads a room by its residue.
**Asymmetry.** On scene, captures: VOC fingerprint, spectral reflectance of residues, ambient acoustic baseline.
**Build.** ESP32-S3 + AS7341 + SGP41 + INMP441 + MLX90614 (surface temp). Transform/Find/Reason.
**Companion software.** Claude-backed "What did this room just witness?" prompt with the JSON of the captures; narrates theory + confidence.
**Why it's fun.** Makes the LLM earn its keep as a modern Watson.

## 8. Kim Possible — mission wristwatch
**Archetype.** Teen hero who takes calls between class.
**Asymmetry.** A wrist console that fuses GPS + HR + mic + mesh-chat with a single trusted team.
**Build.** LILYGO T-Watch S3 (or equivalent) + MAX30102 + INMP441. Represent/Do.
**Companion software.** ESP-NOW + LoRa bridge to a small team's phones; LLM-filtered "only interrupt me if…" rules.
**Why it's fun.** Beeper-to-smartwatch evolution done right.

## 9. Inspector Gadget — absurdist servo drawer
**Archetype.** Comedy device. Over-engineered on purpose.
**Asymmetry.** The fun kind — you can *launch a tiny flag* when your laptop's battery is low.
**Build.** ESP32-S3 + 6 servos + solenoid + USB CDC. Do/Represent.
**Companion software.** A CLI that exposes each servo as a named "gag" tool to a local LLM: `/gogo raise_flag(color='red', reason='inbox zero')`.
**Why it's fun.** Unironically funny; great icebreaker for the community.

## 10. Q (Quartermaster) — the community rapid-prototyping kit
**Archetype.** Gadget maker for other builders. You.
**Asymmetry.** You have the kit that makes kits.
**Build.** A bench-ready ESP32-S3 + a Stemma-QT hub + every sensor in [`../hardware/sensors/`](../hardware/sensors/) connected on a switch. Represent/Generate.
**Companion software.** A local Flask/FastAPI web app that exposes every connected sensor as a tool, with an MCP-style manifest a Claude agent can read. This is literally the bench-twin of the whole repo.
**Why it's fun.** Your build enables *other* persona builds. The multiplier.
