// Kit Sound-Reactive Lights — firmware skeleton
// Level: 10 — First Sense
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// Sound Sensor (analog) + 6812 RGB: the room's noise becomes light.
// All 3.3V-safe. Read the sound sensor on an ADC1 pin (GPIO34) so it keeps working with Wi-Fi on.
// See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int PIN_SOUND = 34;   // analog, ADC1 (input-only pin, ideal for analogRead)
const int PIN_LED   = 4;    // 6812 RGB data line
const int NUM_LEDS  = 8;    // set to your module's pixel count

Adafruit_NeoPixel strip(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);
int baseline = 0;           // ambient "quiet" level learned at boot

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();             // all off

  long sum = 0;             // learn the quiet baseline (~0.4 s)
  for (int i = 0; i < 200; i++) { sum += analogRead(PIN_SOUND); delay(2); }
  baseline = sum / 200;
}

void loop() {
  int raw   = analogRead(PIN_SOUND);          // 1) sense (0..4095)
  int level = abs(raw - baseline);            // 2) transform -> deviation from quiet
  Serial.printf("sound raw:%d level:%d\n", raw, level);

  int lit = map(constrain(level, 0, 1500), 0, 1500, 0, NUM_LEDS);  // 3) distill
  uint32_t hue = strip.ColorHSV((millis() / 10) % 65536, 255, 200);
  for (int i = 0; i < NUM_LEDS; i++)           // 4) represent
    strip.setPixelColor(i, i < lit ? hue : 0);
  strip.show();
  delay(20);                                   // ~50 Hz refresh
}
