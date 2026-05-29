// Kit RFID Doorman — firmware skeleton
// Level: 20 — Connected Object
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// RC522 RFID reader (3.3V ONLY) + 9G Servo (5V) + Active Buzzer: a known card opens the "door".
// RC522 must be powered at 3.3V, never 5V. The servo is a 5V part: power it from the expansion
// board's 5V rail with a common ground. See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

const int PIN_SS     = 21;   // RC522 SDA/SS
const int PIN_RST    = 22;   // RC522 RST (on the S3 pick a valid pin — no GPIO22)
const int PIN_SERVO  = 13;   // 5V servo signal (power from the expansion 5V rail)
const int PIN_BUZZER = 4;    // active buzzer (on/off)

MFRC522 rfid(PIN_SS, PIN_RST);
Servo door;

// Paste your White Card / Key Fob UID here after reading it from serial on the first scan:
byte known[4] = {0xDE, 0xAD, 0xBE, 0xEF};

void beep(int ms) { digitalWrite(PIN_BUZZER, HIGH); delay(ms); digitalWrite(PIN_BUZZER, LOW); }

bool matches(byte *uid, byte len) {
  if (len != 4) return false;
  for (int i = 0; i < 4; i++) if (uid[i] != known[i]) return false;
  return true;
}

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23, PIN_SS);   // SCK, MISO, MOSI, SS
  rfid.PCD_Init();
  pinMode(PIN_BUZZER, OUTPUT);
  door.attach(PIN_SERVO);
  door.write(0);                   // closed
  Serial.println("doorman ready - tap a card.");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { delay(50); return; }

  Serial.print("UID:");            // 1) sense — copy this into known[] to enrol a card
  for (byte i = 0; i < rfid.uid.size; i++) Serial.printf(" %02X", rfid.uid.uidByte[i]);
  Serial.println();

  if (matches(rfid.uid.uidByte, rfid.uid.size)) {   // 2) reason: trusted token?
    Serial.println("ACCESS GRANTED");
    beep(80); delay(60); beep(80);                  // 3) do: happy double-beep + open
    door.write(90); delay(1500); door.write(0);
  } else {
    Serial.println("DENIED");
    beep(500);                                       // one long deny beep
  }
  rfid.PICC_HaltA();
  delay(300);
}
