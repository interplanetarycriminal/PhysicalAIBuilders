# Displays & Represent add-ons

Choose a display for its **emotional contract** as much as its pixel count. See [`../../capabilities/represent.md`](../../capabilities/represent.md).

## Part catalog

| Part | Kind | Interface | Feel |
|---|---|---|---|
| **SSD1306 128×64** | Monochrome OLED | I²C / SPI | Technical, terminal-feel, classic status. |
| **SSD1309 / SSD1327** | Larger / grayscale OLED | SPI | More elegant than 1306 with still-technical vibe. |
| **ST7789 240×240 / 240×320** | Color IPS TFT | SPI | General-purpose rich UI; LVGL friendly. |
| **ILI9488 / ILI9341** | 320×480 color TFT | SPI / 8-bit parallel | Classic hobby color LCD. |
| **GC9A01 round 240×240** | Round IPS | SPI | Watch / dial feel. |
| **Waveshare 2.9" / 4.2" / 7.5" e-paper** | E-paper | SPI | Archival, calm, "read me once" — perfect for distilled output. |
| **Badger2040 / Lilygo T5 e-paper** | Integrated e-paper + MCU | — | Turnkey e-paper wearable badge. |
| **SH1107 128×128** | OLED | I²C / SPI | M5Stack's go-to. |
| **WS2812 / SK6812 NeoPixel** | RGB/RGBW LED | Single-wire | Ambient, alive, signal without text. |
| **APA102 / SK9822** | RGB LED with clock | SPI | Data+clock = smoother for fast animation. |
| **64×64 HUB75 RGB panel** | RGB matrix | Parallel HUB75 | Street-art / shop-sign / clock vibe. |
| **MAX7219 8×8 red matrix** | Monochrome matrix | SPI | Old-school glowing dot board. |
| **Nokia 5110 PCD8544** | Reflective LCD | SPI | Retro, sunlight-visible, low power. |
| **Memory-LCD (Sharp)** | Reflective memory-in-pixel | SPI | Incredibly low power, crisp — elegant wearables. |
| **LED 7-segment (TM1637)** | Digits | 2-wire | Clock / counter feel. |
| **LCD 1602 / 2004 character** | Backlit character LCD | I²C (via PCF8574) | Industrial / terminal / beginner. |

## Emotional contract cheatsheet

| Display | Says |
|---|---|
| e-paper | "read this, then act" |
| SSD1306 | "look here for status" |
| ST7789 w/ LVGL | "use me like an appliance" |
| HUB75 | "I live in this space, see me" |
| NeoPixel | "the vibe changed" |
| Memory-LCD | "I'm on your wrist, trust me" |
| 7-segment | "a number matters right now" |

## Interface notes

- **NeoPixel timing** is strict; use the ESP32's RMT peripheral (via `Adafruit_NeoPixel` or `FastLED`), not `digitalWrite`.
- **TFT DMA** via LovyanGFX / TFT_eSPI hits ~40 fps on an ESP32-S3.
- **E-paper refresh cost**: full refresh is slow (1–5 s) and visually busy; use partial refresh for clocks.
- **HUB75 panels want >5A at 5V** for full brightness — proper power matters.

## Where to buy

- **Adafruit / SparkFun / Pimoroni** for breakouts.
- **Waveshare** direct for e-paper panels.
- **LILYGO / M5Stack** for integrated screen boards.
- **AliExpress** for NeoPixel strips and HUB75 panels.
