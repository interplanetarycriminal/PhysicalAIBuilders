// Kit Joystick Arcade — firmware skeleton
// Level: 20 — Connected Object
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// Joystick (2 analog axes + button) + HT16K33 8x8 dot matrix: steer a glowing dot.
// All 3.3V-safe. Axes on ADC1 (GPIO34/35) so they survive Wi-Fi. I2C on SDA21/SCL22
// (on the S3 there is no GPIO22 -> Wire.begin(8, 9)). See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

const int PIN_X  = 34;   // ADC1, input-only
const int PIN_Y  = 35;   // ADC1, input-only
const int PIN_SW = 32;   // button, active low

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
int x = 3, y = 3;        // cursor position on the 0..7 grid

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SW, INPUT_PULLUP);
  Wire.begin(21, 22);    // classic default; S3: Wire.begin(8, 9)
  matrix.begin(0x70);
}

void loop() {
  int rx = analogRead(PIN_X);                    // 1) sense (0..4095)
  int ry = analogRead(PIN_Y);
  bool pressed = (digitalRead(PIN_SW) == LOW);

  // 2) transform: deflection past a deadzone steps the cursor one cell
  if (rx < 1500 && x > 0) x--;
  if (rx > 2600 && x < 7) x++;
  if (ry < 1500 && y > 0) y--;
  if (ry > 2600 && y < 7) y++;
  Serial.printf("x:%d y:%d btn:%d\n", x, y, pressed);

  matrix.clear();                                // 4) represent
  matrix.drawPixel(x, y, LED_ON);
  if (pressed) matrix.drawRect(0, 0, 8, 8, LED_ON);   // press = frame flash (hook for "fire")
  matrix.writeDisplay();
  delay(120);                                    // step rate
}
