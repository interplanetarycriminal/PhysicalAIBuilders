# Positioning sensors

GPS/GNSS, UWB, magnetometers, pressure altimetry, Wi-Fi fingerprinting. The "where am I, and where are you?" stack.

## Part catalog

| Part | Kind | Interface | Note |
|---|---|---|---|
| **u-blox NEO-6M** | GPS L1 | UART | Cheap, widely taught; ~2.5 m CEP. |
| **u-blox NEO-M8N** | GPS/GLONASS/Galileo | UART | Better fix reliability; much faster cold start. |
| **u-blox NEO-M9N** | Multi-GNSS L1 | UART / I²C | Modern; excellent indoors-adjacent. |
| **u-blox ZED-F9P** | RTK L1/L2 | UART / SPI | cm-level RTK; pair with NTRIP caster or a base station. |
| **ATGM336H** | GPS/BDS | UART | Cheap ATOM-compatible GNSS. |
| **DWM1000 / DWM3000 (UWB)** | Ranging | SPI | Decipoint accuracy between tags+anchors. |
| **BMP388 / BMP581 / LPS22HB** | Pressure altimetry | I²C / SPI | ~10 cm altitude resolution indoors; great for floor detection. |
| **QMC5883L / HMC5883L / LIS3MDL** | Magnetometer | I²C | Compass; must calibrate for local distortion. |
| **Wi-Fi BSSID scan** | Fingerprinting | ESP32-native | Indoor location via learned AP-signal map. |
| **Ultrasonic beacon array** | Indoor positioning | Analog | DIY indoor GPS via ultrasonic TDOA. |

## Interface notes

- GNSS modules emit NMEA over UART. Parse with TinyGPS++ (Arduino) or `pynmea2` equivalents in MicroPython.
- UWB needs 3–4 anchors for 2-D positioning; calibration is where it gets interesting.
- Always combine pressure + magnetometer + IMU for real-world dead reckoning.

## Capability contribution

- **Find**: where it is; which floor; who is close.
- **Transform**: lat/lon → grid cell / POI label.
- **Distill**: trajectory → daily path summary.
- **Reason**: positional + temporal → activity inference.
- **Represent**: map overlay, compass needle, haptic compass on wristband.

## Pairs with (asymmetry combos)

- **ZED-F9P RTK + IMU + LoRa** — survey-grade plant-position logger (arborist / vineyard / salmon-run).
- **UWB anchors + HRV (wristband)** — stress-by-location heatmap of your own home or office.
- **BMP388 + IMU indoors** — know which floor of a building you're on; combine with ESP-NOW to find friends.
- **GNSS + LoRa** — long-range asset / pet / falcon tracker.
- **Wi-Fi BSSID fingerprint + LLM** — "I think you're in the basement" without any external positioning beacons.

## Where to buy

- **u-blox via Sparkfun / Adafruit / DigiKey** for GNSS (avoid counterfeits on AliExpress for RTK).
- **Decawave / Qorvo via DigiKey** for UWB modules.
- **Pololu** for clean magnetometer boards.

## Gotchas

- **GNSS cold start** outdoors can take minutes; use Assist-GPS data (AGNSS) or Galileo E1B where available.
- **Multipath indoors** kills GNSS quickly; indoors → UWB or fingerprinting.
- **Magnetometer warping** by nearby ferrous materials (desks, computer cases) — calibrate in situ.
- **Legal**: high-precision positioning + tracking of people is a consent issue. Opt-in beacons, not covert tags.
