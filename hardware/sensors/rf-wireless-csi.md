# RF / wireless sensing (including Wi-Fi CSI)

The ESP32 can listen to the radio environment in ways almost no other hobbyist chip can. **Wi-Fi Channel State Information (CSI)** in particular turns the ESP32 into a through-wall human-activity sensor — no camera, no mic.

## Built-in capabilities

| Capability | Variants | Note |
|---|---|---|
| **Wi-Fi CSI** | ESP32 (original), ESP32-S3 (partial) | Expose per-packet subcarrier amplitude/phase via `esp_wifi_set_csi_rx_cb`. |
| **BLE scanning** | All | Sniff advertising packets — discover phones, AirTags, BLE beacons. |
| **ESP-NOW** | All | Espressif's low-overhead broadcast protocol; sub-10 ms, meshable. |
| **Wi-Fi promiscuous** | ESP32, S2 | Sniff frames in monitor-ish mode; rate-limited but functional. |
| **Wi-Fi station + AP** | All | Simultaneous STA+AP makes setup wizards trivial. |

## External radios worth pairing

| Part | Band | Interface | Use |
|---|---|---|---|
| **SX1262 (LoRa)** | 868 / 915 MHz | SPI | Long-range low-bandwidth; Meshtastic / LoRaWAN. |
| **SX1280** | 2.4 GHz LoRa | SPI | 2.4 GHz LoRa — global band, shorter range but licence-free worldwide. |
| **nRF24L01+** | 2.4 GHz | SPI | Cheap ≤2 Mbps; classic DIY mesh; sub-$2. |
| **RFM69** | 433 / 868 / 915 MHz | SPI | FSK / OOK, long range, lower data rate. |
| **DWM1000 / DWM3000 (UWB)** | 3.5–6.5 GHz UWB | SPI | Precise ranging (~10 cm); anchor+tag positioning. |
| **RTL-SDR** | 25 MHz – 1.7 GHz (RX) | USB | Paired with a Pi/host; ESP32 coordinates. |
| **433 MHz / 315 MHz RX/TX modules** | ISM | GPIO | Cheap capture/replay of key fobs, weather stations, doorbells. |

## CSI — the superpower

Wi-Fi channel-state information gives you, per packet, the complex amplitude of each OFDM subcarrier. Because humans and pets perturb the radio environment, CSI encodes:

- **Presence** — is someone in the room?
- **Count** — roughly how many?
- **Motion** — are they moving?
- **Breathing rate** — sub-Hz periodic CSI modulation correlates with chest expansion.
- **Gait / activity class** — sitting / walking / falling, via TinyML on CSI features.

All of this **through walls**, **without a camera**, **without a mic**.

## Capability contribution

- **Find**: presence, direction, identity (by device fingerprint).
- **Transform**: raw RF → subcarrier features → classifier input.
- **Distill**: "3 people in the living room since 19:40".
- **Reason**: cross-ref BLE MAC with known devices → "Alex just got home".
- **Represent**: a quiet ambient dashboard of who/where.

## Pairs with (asymmetry combos)

- **ESP32 CSI + LD2410B mmWave** — CSI is through-wall but noisy; mmWave is local and clean. Fuse.
- **ESP32 CSI + microphone** — classify activity and corroborate with ambient sound.
- **BLE scan + e-paper badge** — a party icebreaker; see [`../../projects/party-icebreaker-auralens/`](../../projects/party-icebreaker-auralens/).
- **LoRa mesh + GPS** — distributed falconry / drone-follow / wildlife tracker beyond Wi-Fi range.
- **UWB anchors (4) + tag** — decimeter-accuracy indoor positioning; pair with a VLM for context.

## Where to buy

- **Heltec, LILYGO, RAK** for ESP32+LoRa boards.
- **Adafruit / SparkFun** for UWB breakouts.
- **DigiKey / Mouser** for Semtech SX12xx.
- **AliExpress** for nRF24L01+ and 433 MHz modules (cheap & plentiful).

## Gotchas

- **Legal bands** matter — 915 MHz is US, 868 MHz is EU. Buy the right variant.
- **CSI is noisy** — needs careful bandpass filtering and per-deployment calibration.
- **BLE MAC randomization** makes device tracking unreliable on modern phones; rely on advertising data patterns, not MAC.
- **Ethics of RF sensing** is high-stakes. Re-read [`../../framework/information-asymmetry.md`](../../framework/information-asymmetry.md) §"Ethics of Asymmetry" before you point RF sensing at someone's home.
