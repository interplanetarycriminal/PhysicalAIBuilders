# Safety & GPIO — the working limitations

> The canonical "what will fry a board" and "which pins are safe" page. Every kit-only project and
> the [ESP32 variants page](../hardware/esp32-variants.md) link here. Distilled from the
> [facilitator pack](../MAY29th_README_PhysicalAIBuilders.md) §2. Print it as a one-pager.

## The One Rule, read it aloud first

**ESP32 GPIO pins are 3.3V logic and are NOT 5V tolerant.** 5V parts (ultrasonic, relay, motor,
fan, gas/alcohol/steam sensors) get power from the **5V rail**, and their **signal never goes
straight into a GPIO**. When in doubt, power the sensor from 3.3V.

## The 5 rules to read aloud before anyone plugs in

1. **3.3V is the law.** Putting 5V into a GPIO can degrade or kill the pin and sometimes the chip.
   Power sensors from 3.3V unless the part needs 5V; if it does, keep its signal out of the GPIO or
   drop it with a divider.
2. **Never plug or unplug parts while powered.** Pull the USB cable first, wire it, then power up.
   Hot-plugging is the classic way to short two pins and fry something.
3. **Check polarity before power.** V→V (3V3 or 5V), G→GND, Signal→the GPIO you chose. Reversing V
   and G destroys a module in seconds. The expansion board's keyed headers prevent this.
4. **One disconnect reflex:** if anything smells hot, gets too hot to hold, or smokes, **pull the
   USB cable and the AA pack immediately**, then investigate. Do not wait and see.
5. **Handle boards by the edges.** Fingers on the PCB edge, not on the chips, pins, or gold contacts.

## Safe GPIO map — ESP32-WROOM-32E (classic, the default board)

- **3.3V logic, not 5V tolerant.**
- **Input-only (no output, no internal pull-up):** GPIO **34, 35, 36, 39**. Great for analog
  sensors, useless for LEDs/relays.
- **Strapping pins (affect boot, avoid for sensors if a board won't flash):** GPIO **0, 2, 5, 12, 15**.
- **Do not use at all (wired to internal flash):** GPIO **6, 7, 8, 9, 10, 11**.
- **Analog with Wi-Fi on:** use **ADC1 = GPIO 32–39**. The ADC2 pins (incl. 0, 2, 4, 12–15, 25–27)
  **stop working when Wi-Fi is active** — this surprises everyone.
- **Safe general-purpose (output/PWM/I²C/etc.):** GPIO **4, 13, 14, 16, 17, 18, 19, 21, 22, 23, 25,
  26, 27**, plus 32/33 for analog or output.
- **Default I²C:** SDA = **21**, SCL = **22**.

## Safe GPIO map — ESP32-S3-WROOM-1 (the BLE / native-USB board)

- **3.3V logic, not 5V tolerant.**
- **GPIO range:** 0–21 and 26–48. **GPIO 22, 23, 24, 25 do not exist on the S3.** The big gotcha:
  any classic example using **GPIO22 (e.g. I²C SCL) will not work on the S3.** Remap I²C, e.g.
  `Wire.begin(8, 9)` (SDA=8, SCL=9), or pick any valid free pair.
- **Native USB pins (leave alone):** GPIO **19 (D−)** and **20 (D+)**.
- **Strapping pins:** GPIO **0, 3, 45, 46**.
- **Do not use (internal flash; PSRAM on PSRAM modules):** GPIO **26–32**, and on Octal-PSRAM
  variants (the common N8R8) also **33–37**. If an S3 sketch fails only on those high pins, that's
  why — move to GPIO **4–18** or **38–42**.
- **Analog with Wi-Fi on:** use **ADC1 = GPIO 1–10**. ADC2 (GPIO 11–20) conflicts with Wi-Fi.
- **Serial over USB:** enable **USB CDC On Boot** or the serial monitor stays blank.

## Voltage & power (the stuff that fries boards)

- **Which kit parts are 5V:** SR01/HC-SR04 ultrasonic, 5V relay, 130 motor, fan, analog gas,
  alcohol, steam, and DS1307 (power). For the analog 5V sensors, the output can swing above 3.3V, so
  **put a 1k + 2k divider on the signal** before a GPIO, or read it through the expansion board's
  logic-safe header. For the ultrasonic, the **Echo line specifically needs a divider**; Trig is
  fine straight from a GPIO.
- **USB current limit & brownout:** an ESP32 can spike to **400–500 mA** when Wi-Fi starts. Many
  laptop ports/hubs supply less. Symptoms of starvation: random resets, `rst:0xf (BROWNOUT_RST)` in
  the serial monitor, failed uploads. Fix with a short, thick, known-good **data** cable (not
  charge-only), a direct laptop port or powered hub, and by powering servos/motors/relays/LED strips
  from the **expansion board + AA pack**, not the 3.3V pin.
- **Never feed >5V into the 5V/VIN pin**, and never feed voltage into the 3.3V pin unless it is a
  regulated 3.3V source (that bypasses the board's protection).

## The 1k + 2k divider (for the ultrasonic Echo and 5V analog outs)

```
5V signal ──[ 1k ]──┬──► GPIO (≈3.3V)
                    │
                  [ 2k ]
                    │
                   GND
```

`Vout = Vin × 2k / (1k + 2k) = 5V × 0.667 ≈ 3.3V`. Trig is driven *by* the ESP32 (3.3V out is fine);
only **Echo** (driven by the sensor at 5V) needs the divider.

## Flashing gotchas (say these out loud at setup)

- **Classic WROOM-32E / Keyestudio Plus:** needs the **CP2102 (or CH340) USB driver**. If upload
  hangs on `Connecting... _ _ _`, **hold BOOT**, release once the percentage starts.
- **ESP32-S3:** native USB. Set **USB CDC On Boot: Enabled** or the serial monitor stays blank. If
  upload fails, enter download mode: **hold BOOT, tap RESET, release BOOT.** Devkits often expose two
  USB ports — either flashes, pick whichever shows a port.
- **Serial monitor baud: 115200.**

## ESD, heat, batteries — kept light but real

- **ESD:** touch a grounded metal object (the laptop chassis) before handling bare boards; keep
  parts in anti-static bags until needed. Brisbane humidity helps, but A/C dries the air — keep the
  habit.
- **Heat:** a regulator running mildly warm is normal. Too hot to hold or rapidly heating → unplug
  now. Usual causes: reversed polarity, a short, a stalled servo, a motor wired straight to a pin, a
  sensor on the wrong rail. A buzzing, hot servo is usually jammed against a stop — cut power and
  free it.
- **Battery (6-slot AA holder):** alkaline AA only (not lithium). Observe polarity, don't short the
  terminals, don't mix old and new cells, remove the pack if it gets warm.

## Quick triage if something goes wrong

1. Pull the USB cable and the AA pack. 2. Look for scorch marks, smell, a hot part. 3. Check polarity
and shorts before re-powering. 4. Re-power after **one fix at a time**. 5. If a board is confirmed
dead, swap it — don't burn session time on a resurrection.
