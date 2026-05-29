// Kit Tripwire Alert — firmware skeleton
// Level: 10 -> 20
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// PIR Motion + 5V Relay + Laser: an invisible line that knows when it's crossed and acts on it.
// The relay coil is a 5V load: power it from the expansion board's 5V rail (NOT the 3.3V pin),
// with a common ground. See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>

const int PIN_PIR   = 13;   // motion in (interrupt-capable)
const int PIN_RELAY = 4;    // relay IN; coil powered from 5V via the expansion board
const int PIN_LASER = 18;   // "armed" beam / visible indicator (never aim at eyes)

volatile bool motion = false;
unsigned long alarmUntil = 0;

void IRAM_ATTR onMotion() { motion = true; }

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  digitalWrite(PIN_LASER, HIGH);                 // armed
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), onMotion, RISING);
  Serial.println("armed.");
}

void loop() {
  if (motion) {                                  // 1) sense (set by the ISR)
    motion = false;
    Serial.println("MOTION - tripwire crossed!");
    alarmUntil = millis() + 3000;                // 2) distill: hold the alarm 3 s
  }

  bool alarming = millis() < alarmUntil;
  digitalWrite(PIN_RELAY, alarming ? HIGH : LOW);                 // 3) do: switch the 5V load
  digitalWrite(PIN_LASER, alarming ? (millis() / 100) % 2 : HIGH); // blink while alarming
  delay(10);
}
