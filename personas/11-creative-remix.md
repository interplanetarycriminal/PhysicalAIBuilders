# Creative remix

Ten personas for whom the *entire asymmetry* is software imagination. Every persona here has at-hand hardware that would be "boring" in another's hands — and turns it into something impossible with a software twin.

Read this file alongside [`../framework/software-as-imagination.md`](../framework/software-as-imagination.md) and [`../framework/riff-book.md`](../framework/riff-book.md). These personas don't invent new sensors; they **invent new meanings**.

---

## 101. The Stage Walk-By Magician
**Archetype.** Installation artist who turns a hallway into a theatre.
**Asymmetry.** Passers-by become pirates on a rocking ship on the wall — visible only from the waist up; waves match their gait; a couple becomes a galleon; a running child becomes a seabird.
**Build.** ESP32-CAM or cheap USB camera + a browser game engine on a laptop + projector. Find/Transform/Generate.
**Companion software.** A p5.js or three.js scene that receives silhouette + velocity events over WebSocket. See [`../projects/pirate-stage-walkby/`](../projects/pirate-stage-walkby/).
**Why it's fun.** The sensor barely matters; the imagination is everything. Strangers grin.

## 102. The Handshake Across the Map
**Archetype.** Long-distance friend / lover / colleague.
**Asymmetry.** When you and someone across the city touch your respective capacitive pads within 2 s, both pads' lamps pulse the same color — a signal nobody else sees.
**Build.** 2× XIAO ESP32-C3 + TTP223 capacitive touch + NeoPixel + MQTT relay.
**Companion software.** A tiny broker + a shared-secret pairing; consent-gated. *Social* mode from the riff book.
**Why it's fun.** The most private possible "hi" — sensor event re-meaning'd as love letter.

## 103. The Home Tamagotchi
**Archetype.** The household's storyteller.
**Asymmetry.** Every sensor in the house feeds a single virtual creature whose mood, hunger, stories, and life events are driven by real-world data.
**Build.** Any existing ESP32 sensors + a home server + an LVGL screen for the creature.
**Companion software.** LLM-scripted creature that narrates the house's life in first person (Creature-POV). *World / RoleReversal* modes.
**Why it's fun.** A family falls in love with a digital being whose heartbeat is their fridge door.

## 104. The Room Narrator
**Archetype.** Audiobook-voiced roommate.
**Asymmetry.** Every event — you walk in, the kettle whistles, the dog sighs — gets narrated over a bedside speaker by a voice you chose (Attenborough-style, noir detective, Shakespearean, Cartman).
**Build.** ESP32 + mic (RMS only) + mmWave + thermal + a TTS-capable companion.
**Companion software.** A Claude prompt-template-per-voice; optional local TTS. *Persona / Narrative* modes.
**Why it's fun.** Your Tuesday becomes a radio drama starring you.

## 105. The Gallery Voting Booth
**Archetype.** Curator / installation artist.
**Asymmetry.** Gallery guests touch capacitive pads with one of N "votes"; software makes each day's gallery-mood from the distribution.
**Build.** 4× capacitive pads per prompt + ESP32 + daily e-paper summary.
**Companion software.** Claude writes the "mood of the gallery today" and a plaque-ready paragraph. *Social / Oracle / Art* modes.
**Why it's fun.** Art and software co-author the show.

## 106. The Escape-Room Designer
**Archetype.** Puzzle maker.
**Asymmetry.** Every mundane object is a sensor event; every sensor event is a puzzle gate.
**Build.** Ten XIAO ESP32-C3s + reed switches, capacitive pads, photoresistors, sound-level detectors; central ESP32-S3 orchestrator.
**Companion software.** A "puzzle state machine" dashboard + LLM-written room narrator. *Game / Persona* modes.
**Why it's fun.** Cheap sensors become a world; a room becomes a ritual.

## 107. The Museum Exhibit Whisperer
**Archetype.** Docent who wants exhibits to wake up.
**Asymmetry.** Stand too long in front of a painting and it whispers a secret — its own imagined monologue — into a bone-conduction wristband you borrowed at the door.
**Build.** Ceiling mmWave + ultrasonic per exhibit + BLE-paired wristband.
**Companion software.** Per-exhibit LLM persona; prompts curated by the museum team. *Persona / RoleReversal* modes.
**Why it's fun.** Respectful, patient magic. You leave the museum having been told secrets.

## 108. The Desk-Top Theatre Director
**Archetype.** Writer who gets lonely at their desk.
**Asymmetry.** The kettle, fridge, and cat-feeder run a daily soap opera written via LLM from their real-world events; the writer checks in to read the day's episode.
**Build.** 3 ESP32s + cheap sensors; all publish to one MQTT broker.
**Companion software.** An LLM "showrunner" prompt — "write a 5-exchange scene that would fit these events." *MachineTheatre* mode.
**Why it's fun.** Your appliances become ensemble characters; you are their audience.

## 109. The Invisible-Piano Performer
**Archetype.** Music-curious non-musician.
**Asymmetry.** Five capacitive pads on a table become a piano whose scale shifts by the room's CO₂ — dawn scales in the morning, minor in a stuffy evening.
**Build.** 5× capacitive pads + SCD41 + I²S speaker + ESP32-S3.
**Companion software.** Ambient music model on a laptop that treats touches + CO₂ as a live conductor's score. *Art / Environmental* modes.
**Why it's fun.** The room teaches you to play itself.

## 110. The Time-Folded Self
**Archetype.** Self-reflection practitioner.
**Asymmetry.** The device plays you *your past self's* audio snippets and notes when you enter familiar states — not as a safety alert, as gentle narrative reminder.
**Build.** Any wearable + BLE + a curated-by-you library of "dear future self" notes indexed by state signature.
**Companion software.** A state-embedding index (vector DB); Claude orchestrates retrieval. *TimeShift / Oracle* modes.
**Why it's fun.** You become a collaborator with your past. The asymmetry is temporal — *you* know more about *you* than anyone else ever could.

---

## How to riff a new creative persona

1. Pick hardware you already own. (Don't buy anything yet.)
2. Pick a **mode** from the twelve in [`../framework/software-as-imagination.md`](../framework/software-as-imagination.md).
3. Re-name the sensor in a notebook: "this scalar = ___." Be absurd on purpose.
4. Write the opening line of the *story* the build would tell.
5. Sketch the software twin in the companion pattern (see [`../framework/software-hardware-pairing.md`](../framework/software-hardware-pairing.md)).
6. Build, share, add the persona to this file via PR.
