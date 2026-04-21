# Flagship projects — the ladder as 10 buildable things

Ten fully-specified builds, one per level on [the learning ladder](../framework/learning-levels.md). Climb them in order if you're new, or skip to your rung. Each project is a complete buy/build guide:

- **README.md** — level, vibe, BOM (with vendors), wiring, information asymmetry, companion software, ethics, extension paths.
- **firmware/** — ESP32 firmware skeleton (Arduino or ESP-IDF flavor, clearly labeled).
- **companion/** — the software twin (phone app, web dashboard, LLM prompt, agent tool surface).

| Level | Project | Persona tribe | Core stack | Companion software |
|---|---|---|---|---|
| 10 | [`party-icebreaker-auralens`](party-icebreaker-auralens/) | Spies & Shadows | ESP32-C3 + BLE + NeoPixel + OLED | phone webview |
| 20 | [`homebrewer-fermentdoc`](homebrewer-fermentdoc/) | Domestic | ESP32 + DS18B20 + BME280 + Tilt | Wi-Fi + web dashboard |
| 30 | [`ar-garden-oracle`](ar-garden-oracle/) | Domestic | ESP32-S3 + AS7341 + soil moisture | phone AR overlay + LLM |
| **30 (creative track)** | [`pirate-stage-walkby`](pirate-stage-walkby/) | **Creative remix** | ESP32-CAM + WebSocket | **browser scene is the whole project** — passers-by become pirates on rough seas |
| 40 | [`parent-sleep-ally`](parent-sleep-ally/) | Domestic | ESP32 + LD2410B + SCD41 + e-paper | LLM morning summary |
| 50 | [`parkour-flowstate`](parkour-flowstate/) | Sports | XIAO ESP32-S3 Sense + IMU + HR | TinyML + replay app |
| 60 | [`falconer-wingcomm`](falconer-wingcomm/) | Nature | LILYGO T-Beam + GPS + BMP + LoRa | glove display + map |
| 70 | [`beekeeper-hivelink`](beekeeper-hivelink/) | Nature | ESP32-S3 + HX711 + piezo + SEN55 + solar + LoRa | swarm-forecast dashboard |
| 80 | [`ghost-hunter-csi-presence`](ghost-hunter-csi-presence/) | Strange | ESP32 (CSI) + LD2410B + mic | LLM narrator + phone UI |
| 90 | [`wildfire-lookout-pyrosentinel`](wildfire-lookout-pyrosentinel/) | High-stakes | ESP32-S3 + MLX90640 + SEN55 + BME + anemometer + LoRa + solar | ops dashboard + SitRep agent |
| 100 | [`cold-case-sleuth-timefold`](cold-case-sleuth-timefold/) | Strange / Knowledge | multi-site ESP32 mesh + retention DB + agent | Claude-driven investigator |

## Creative track

The ladder above is an **operational** progression — each rung teaches a new engineering skill. But this repo's other axis is **creative imagination** (see [`../framework/software-as-imagination.md`](../framework/software-as-imagination.md)): what story can the software tell with the same raw signal?

The **creative-track** flagship at Level 30 — [`pirate-stage-walkby`](pirate-stage-walkby/) — exists to teach that thesis by example. The ESP32 is trivial; the projected browser scene is the whole build. Walk past and you are a pirate on rough seas; a couple becomes a galleon; a running child becomes a seabird. Do not underestimate how much this one project unlocks in a builder's mind.

Future creative-track flagships (community contributions welcome): a "home Tamagotchi" built from whatever sensors you already own, a "room narrator" with a chosen voice, a handshake-across-the-city pair, an escape-room kit. See [`../personas/11-creative-remix.md`](../personas/11-creative-remix.md) for the seeds.

## Philosophy

- **Every project teaches at its level and the one above.** Enough stretch to grow, not enough to drown.
- **Every project names its asymmetry.** If the README can't answer "what does the wielder know that others don't?", it's not done.
- **Every project names its software twin.** No build is hardware alone; see [`../framework/software-hardware-pairing.md`](../framework/software-hardware-pairing.md).
- **Every project includes ethics.** Re-read [`../framework/information-asymmetry.md`](../framework/information-asymmetry.md) §"Ethics of Asymmetry" before you build anything that senses people.

## Starting a new flagship

Copy [`_template/`](_template/) into a new folder named `<slug>/`, fill in each section, and level-tag the project. The template matches the standard structure so everything in this directory feels cohesive.
