# Knowledge workers & AI builders

Ten personas whose work is thinking — for whom an ESP32 build becomes a second cortex.

---

## 51. The Research Scientist — lab-ambient logger
**Asymmetry.** Your bench history is on tape: every temp, humidity, VOC, light, vibration event, time-stamped.
**Build.** ESP32-S3 + SEN55 + BME280 + IMU + MLX90614 (thermocouple for sample) + piezo (hood airflow). Find/Distill.
**Companion software.** Notebook-style web log with auto-section per experiment; LLM writes the methods paragraph for your paper.
**Why it's fun.** Reviewers will stop asking about conditions.

## 52. The UX Researcher — observation field device
**Asymmetry.** Captures micro-expressions (thermal), speech, and environment while leaving participants unobtrusive.
**Build.** ESP32-S3 + AMG8833 (low-res thermal, privacy-appropriate) + mic + consent LED. Find/Transform.
**Companion software.** Participant-owned app lets them review & redact before upload. Consent is visible.
**Why it's fun.** Research rigor + ethics baked into hardware.

## 53. The Investigative Journalist — source-shield
**Asymmetry.** Detect active cameras, mics, and cellular IMSI catchers in the room.
**Build.** ESP32 + RF band scanner + ultrasonic detector + camera-lens glint IR LED + Faraday pocket. Find/Represent.
**Companion software.** Phone app; e-paper shows a "clean room" or "compromised" glyph. See [`../projects/investigative-journalist-sourceshield/`](../projects/investigative-journalist-sourceshield/).
**Why it's fun.** Quiet, necessary, unimpeachable.

## 54. The IP Lawyer — ambient patent scanner
**Asymmetry.** Point at a device; get the likely patents that cover its features.
**Build.** XIAO ESP32-S3 Sense + mic + OV2640 + BLE sniffer. Find/Reason.
**Companion software.** Phone app that POSTs the multimodal snapshot + descriptors to a LLM + patent search pipeline.
**Why it's fun.** A fun tool that disguises a productivity win.

## 55. The Therapist — session HRV companion (client-owned)
**Asymmetry.** Client sees their own HRV + skin-temp in real time during a session, builds interoception.
**Build.** Wristband XIAO ESP32-C3 + MAX30102 + MAX30208. Transform/Distill/Represent.
**Companion software.** Client-owned app; therapist sees nothing unless client shares a summary.
**Why it's fun.** Hands agency to the client; therapeutic intent first.

## 56. The Pediatrician — waiting-room air + noise
**Asymmetry.** Clinic-level ambient air + mic RMS heatmap to allocate staff.
**Build.** ESP32 + SEN55 + mic (RMS only, no capture) in each room. Distill.
**Companion software.** Receptionist dashboard + recommendations from an LLM: "Room 3 is getting loud, escalate."
**Why it's fun.** Measurable improvement in the feel of a clinic.

## 57. The Forensic Accountant — meeting-log device
**Asymmetry.** Legally-compliant ambient log of meetings (consent on, visible indicator) ties events to minutes.
**Build.** ESP32-S3 + mic + LED consent indicator + local encryption. Transform/Find.
**Companion software.** Secure upload to Whisper; an LLM cross-refs with accounting entries.
**Why it's fun.** Turns the "who said what" argument into an evidence-based conversation.

## 58. The Patent Strategist — innovation scanner
**Asymmetry.** Walks a trade show and scans 100 booths; gets a report by dinner.
**Build.** XIAO ESP32-S3 Sense + BLE + Wi-Fi scan + OV. Find/Distill.
**Companion software.** Claude "trade-show field agent" that summarizes the capture trail.
**Why it's fun.** The trade show becomes a searchable corpus.

## 59. The Analytics Engineer — focus-state ring
**Asymmetry.** Sees own "deep work" blocks confirmed by biometry, not self-report.
**Build.** XIAO ESP32-C3 + MAX30102 + keypress counter over BLE. Distill.
**Companion software.** Toggl-style timer that auto-tags *measured* focus, not claimed focus.
**Why it's fun.** Best productivity hack is the honest one.

## 60. The AI Builder (you!) — prompt-sensor loop
**Asymmetry.** Your environment is a tool for your LLM, not a passive backdrop.
**Build.** Q-branch bench (persona #10) + a local MCP server that wraps every sensor as a tool.
**Companion software.** Claude / Code runs with live ESP32 tools; "what is this room trying to tell me?" becomes a natural query.
**Why it's fun.** Collapses the gap between writing code and building reality.
