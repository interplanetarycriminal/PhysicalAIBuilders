// Kit Parking Sensor — firmware skeleton
// Level: 10 — First Sense
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// SR01 / HC-SR04 ultrasonic + Active Buzzer: beeps faster as an object gets closer.
// 5V part: power the SR01 from the 5V rail (expansion board / AA pack), and drop ECHO to
// 3.3V through a 1k+2k divider before GPIO16. See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>

const int PIN_TRIG   = 17;   // 3.3V out from the ESP32 is fine straight to TRIG
const int PIN_ECHO   = 16;   // via 1k+2k divider (the sensor drives this at 5V)
const int PIN_BUZZER = 4;    // active buzzer, on/off

const float MAX_CM = 100.0;  // ignore anything farther than this

float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 30000UL); // 30 ms timeout (~5 m)
  if (us == 0) return MAX_CM;                          // no echo -> treat as far
  return us / 58.0;                                    // ~58 us per cm (round trip)
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  float cm = readDistanceCm();                         // 1) sense
  Serial.printf("distance: %.1f cm\n", cm);

  if (cm < MAX_CM) {
    int gap = map((int)cm, 5, (int)MAX_CM, 60, 800);   // 2) distill: closer -> shorter gap
    gap = constrain(gap, 60, 800);
    digitalWrite(PIN_BUZZER, HIGH); delay(30);         // 3) represent / do
    digitalWrite(PIN_BUZZER, LOW);  delay(gap);
  } else {
    digitalWrite(PIN_BUZZER, LOW);                     // far / no target: silence
    delay(100);
  }
}
