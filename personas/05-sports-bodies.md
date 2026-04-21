# Sports & bodies

Ten personas whose advantage is a cleaner signal from their own body or the system they're in.

---

## 41. The Parkour Athlete — flow-state tracker
**Asymmetry.** Knows exactly which jumps tipped them from flow into fatigue.
**Build.** Wearable XIAO ESP32-S3 Sense + IMU + HRV from MAX30102. Transform/Distill.
**Companion software.** Post-run replay that tags the exact jump where flow broke. See [`../projects/parkour-flowstate/`](../projects/parkour-flowstate/).
**Why it's fun.** Self-knowledge via your own telemetry.

## 42. The Climber — route recall
**Asymmetry.** Every move of every route is logged via IMU + wrist accel — you remember routes you climbed a year ago.
**Build.** Two XIAO ESP32-C3 wristbands + IMU + BLE sync. Transform/Distill.
**Companion software.** Route library with IMU "signature" search: "show me any route that felt like this one".
**Why it's fun.** Climbing nerds (read: all climbers) will love it.

## 43. The Freediver — breath-hold console
**Asymmetry.** Track CO₂ tolerance per dive with a wearable that's watertight.
**Build.** Potted ESP32-C3 + depth sensor + HRV + ambient temp. Transform/Distill.
**Companion software.** Dive coach LLM that writes tomorrow's training plan from today's curves.
**Why it's fun.** Sport where tiny improvements compound into life safety.

## 44. The Ultrarunner — custom pace coach
**Asymmetry.** Pace recommendations that know *your* historical tired-at-mile-40 pattern.
**Build.** LILYGO T-Display + IMU + HR + GPS + BMP (altitude). Distill/Reason.
**Companion software.** A pace model trained on your past races; LLM reads it aloud mid-race via bone conduction.
**Why it's fun.** A coach who's always there but doesn't tire.

## 45. The Surfer — wave & session watch
**Asymmetry.** Count waves caught, time on wave, session quality heatmap.
**Build.** Waterproof ESP32-S3 + IMU + GPS + LoRa. Find/Transform.
**Companion software.** Session replay with "waves caught vs attempted" + daily surf diary.
**Why it's fun.** Non-invasive tracking that doesn't kill the session vibe.

## 46. The E-sports Coach — reaction + HRV viewer
**Asymmetry.** Your team's biometric dashboard shows when a player is tilting before they do.
**Build.** Per-player MAX30102 + GSR strap + XIAO ESP32-C3 → team LAN. Find/Distill.
**Companion software.** Private coach-only dashboard; mic-triggered suggestion ("call time-out").
**Why it's fun.** Competitive edge, ethically delivered.

## 47. The Boxing Cornerman — round-by-round vitals
**Asymmetry.** Between rounds, see HR, HRV recovery, stance-change history.
**Build.** Mouthguard + chest-strap with XIAO ESP32-C3 + BLE to cornerman's phone.
**Companion software.** A coach app that overlays recovery curves with round scores.
**Why it's fun.** Takes corners from instinct to instinct+data.

## 48. The Yoga Teacher — breath + balance badge
**Asymmetry.** Students see their own breath rate and sway on a quiet e-paper badge.
**Build.** XIAO ESP32-C3 + resp belt + IMU + e-paper. Transform/Distill/Represent.
**Companion software.** A class-wide instructor view; students remain anonymous.
**Why it's fun.** Quiet, respectful, transformative feedback.

## 49. The Cyclist — power + cadence + pack radar
**Asymmetry.** Knows who in the pack is suffering (via HR-share or ESP-NOW beaconing).
**Build.** Seatpost ESP32-S3 + cadence sensor + torque strain gauge + BLE. Transform/Find.
**Companion software.** A pack app over LoRa that shows mutual state — opt-in.
**Why it's fun.** A group-riding superpower built on consent.

## 50. The Rowing Crew Coach — per-seat IMU sync
**Asymmetry.** See which rower is out of phase with the crew — by seat.
**Build.** 8× XIAO ESP32-C3 + IMU in each seat + ESP-NOW master. Transform/Find.
**Companion software.** Coach's phone app with phase diff bars + coxswain narration.
**Why it's fun.** The oldest team sport finally gets a per-seat phase readout.
