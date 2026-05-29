// Kit Weather Station — firmware skeleton
// Level: 20 — Connected Object
// Board: ESP32-WROOM-32E (classic).  Paired software: ../companion/README.md
//
// XHT11 (DHT11-compatible) temp/humidity + LCD_128x32 (I2C) readout.
// All 3.3V-safe. DHT data on GPIO4; I2C on SDA21/SCL22 (on the S3 there is no GPIO22 -> remap,
// e.g. Wire.begin(8, 9)). See ../../build-night/safety-and-gpio.md.

#include <Arduino.h>
#include "DHT.h"
// #include "lcd128_32_io.h"  // Keyestudio kit lib — install from the kit "Libraries" zip

#define DHTPIN  4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// lcd128_32 lcd(/*SDA*/21, /*SCL*/22, /*addr*/0x3F, /*cols*/16, /*rows*/2);

void setup() {
  Serial.begin(115200);
  dht.begin();
  // lcd.Init(); lcd.Clear();
}

void loop() {
  float t = dht.readTemperature();        // 1) sense (deg C)
  float h = dht.readHumidity();           //    sense (%RH)

  if (isnan(t) || isnan(h)) {             // DHT11 occasionally returns NaN — retry, don't print junk
    Serial.println("DHT read failed, retrying...");
    delay(1000);
    return;
  }
  Serial.printf("temp:%.1fC  humidity:%.0f%%\n", t, h);   // 2) represent (serial — the first win)

  // 3) represent (LCD) — finish with Claude Code using the kit lib's API:
  // char line[17];
  // snprintf(line, sizeof(line), "T:%.1fC H:%.0f%%", t, h);
  // lcd.Cursor(0, 0); lcd.Display(line);

  delay(2000);                            // DHT11 is slow; ~0.5 Hz is plenty
}
