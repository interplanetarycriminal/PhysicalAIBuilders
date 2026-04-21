# The Riff Book

> A gallery of **sensor-event → creative-software response** pairings. Each row is a seed, not a build — pick one, stretch it, ship your own.

The riff book exists because imagining the software twin is **the** creative act of a Physical-AI build. The hardware is cheap and the sensor readings are raw vocabulary; the riff is the sentence that gives them meaning. See [`software-as-imagination.md`](software-as-imagination.md) for the twelve modes these riffs draw from.

---

## How to read a riff

Each row is: **[sensor event]** → **[mode]** → **[software response]**, in one line. Mode labels are shorthand for the twelve in the manifesto: *Persona, World, Game, Narrative, Environmental, Social, TimeShift, Physics, Art, Oracle, RoleReversal, MachineTheatre*.

---

## Capacitive / humidity / moisture probe

*(remember: any cheap moisture probe will also respond to a finger, a breath, a spilled drink — raw vocabulary, endless meaning)*

| Sensor event | Mode | Creative software response |
|---|---|---|
| Finger touches probe | Social | Paired probe across the house/city pulses its LED in the same color; "ambient handshake." |
| Two fingers touch two probes within 2 s | Social | Both desks' lamps blink once — no words, just "hi." |
| Prolonged touch, rate-of-change curve | Game | Mapped to "spell power"; a slow lingering touch = long incantation, a tap = fireball. |
| Dry reading | Persona | A cactus plush whispers "I'm fine :(" via BLE. |
| Sudden high reading | Oracle | "The well of the hour has risen; expect gossip." Written to e-paper by an LLM. |
| Touch pattern (morse-like) | Game | A hidden safe-drawer pops open if the pattern matches. |
| Five probes in a row | Art | Invisible piano; software chooses the scale per day. |
| Probe in a potted plant + touch | RoleReversal | The plant "introduces itself" — it's decided the human approaching is a guest in its story. |

---

## IMU (accel + gyro)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Double tap on desk | Persona | "Rick Sanchez" voice answers whatever question you were about to ask. |
| Shake vigorously | World | Your home Tamagotchi wakes up and demands a haiku. |
| Tilt held 30° for 2 s | Game | Cast "ice shard" in the AR RPG running on your phone. |
| Pitch over time | Narrative | Walked-with-the-wearable log → wildlife-doc narration, Attenborough voice. |
| Stillness > 5 min (while awake per HRV) | TimeShift | The device plays *last* week's stillness-at-this-time as proof you also do this on Tuesdays. |
| Freefall | Physics | Room camera feed "inverts gravity" on-screen for 3 s. |
| Spin around 360° | Art | On a projected wall, a chalk circle draws itself around you. |
| Gait rhythm | MachineTheatre | Your wristband and your dog's collar IMU "write haiku to each other" on a shared e-paper. |

---

## Microphone (no recording; RMS / FFT / detection)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Clap once | Environmental | Room lights shift from stadium blue → sunset gold for 30 s. |
| Clap twice | Game | Lives a "ping" into a game running on the wall; other rooms' sensors can reply. |
| Kettle whistle detected | Persona | An old-timey BBC announcer says "tea, chaps." |
| Cough | TimeShift | Device plays back last 5 coughs in this week, each with a "crowd laugh" overdub. |
| Fridge-door-close thump | World | Your Stardew-Valley-style kitchen sim closes its in-game fridge. |
| Baby cry (TinyML) | Narrative | A quiet, serious voice on a bedside speaker: "Act II has begun." |
| Footsteps on stairs | Social | Partner's desk lamp dims — they "hear you coming." |
| FFT spectrogram | Art | Drives a 64×64 HUB75 matrix; bedroom becomes a sleeping spectrogram. |
| Tonality shift (beehive-style) | Oracle | "The hive is tuning up — the queen may leave on a Thursday." |
| Sub-audible < 20 Hz rumble | Physics | On-screen furniture ripples for 2 s. |

---

## Camera (silhouette / person-detect / color)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Person walks L → R past camera | Environmental | On the projected wall, only their upper half appears, rendered as a pirate with a rocking boat hull below, waves matching gait direction + speed. (See [`../projects/pirate-stage-walkby/`](../projects/pirate-stage-walkby/).) |
| Couple passes | Environmental | Two pirates sharing one galleon; captain's wheel between them. |
| Familiar face (Person Sensor slot) | Persona | Cartman line: "Ay, it's [name], I hate [name]." (Affectionate version.) |
| Shirt color = red | Game | Red team scores. 7-segment scoreboard ticks. No rules are explained. |
| Sustained presence at desk | World | Your home's Sims-style "workday" clock advances. |
| Person stops, looks at camera | Oracle | Thermal printer below spits a tarot card + one-line reading. |
| Multiple people cluster | Art | Their silhouettes are painted into a daily Van-Gogh mural at midnight. |
| Child-sized silhouette | Persona | A Shrek line plays. Adults get Wallace. |
| Ceiling camera, wall-empty | MachineTheatre | The room becomes an empty theater; the webcam writes a monologue to itself about loneliness. |
| Motion direction = towards door | Narrative | LLM appends a "and [name] headed out" beat to the day's diary. |

---

## Thermal imaging (MLX90640 24×32)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Hand-shape hotspot | Art | On-screen, the hand shape leaves a "handprint" on a virtual cave wall. |
| Forehead-temperature estimate | Oracle | "You're hot-headed today — write before noon." (For self; never diagnose others.) |
| Pet warm spot on couch | Persona | "The couch reports Max is here." |
| Thermal gradient on wall | RoleReversal | The wall itself becomes the protagonist: "I've been cold since the sun moved." |
| Two heat sources together | World | Your Tamagotchi logs a "meeting" in its diary. |
| Sudden cold spot in a warm room | Physics | Desktop wallpaper grows a layer of frost where the cold spot maps. |

---

## mmWave (LD2410B presence + motion + range)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Presence begins | Social | Partner in another room receives a one-LED "you're not alone" pulse. |
| Motion during deep night | TimeShift | Plays the one-liner you wrote *last* time you were up at 03:00 ("stop catastrophizing"). |
| Distance = 1–2 m | Persona | The room's AI-narrator uses an "up close" register; above 2 m, a "down the hall" one. |
| Long stillness | World | Your home-sim lights up its library room. |
| Two-target tracking (LD2450) | Game | A stealth-game: avoid being in line-of-sight between the two dots. |
| Presence + CO₂ rising | Oracle | "The council is deliberating; the air grows heavy." |

---

## BME680 / SEN55 (air, VOC, PM, RH, T)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Cooking VOC spike | Persona | Julia Child voice: "Mmm. Someone's making butter." |
| Air pressure drop | Oracle | Writes an "ill omens" tanka to e-paper. |
| CO₂ > 1200 | Environmental | Room's Hue lights shift cooler by 200 K until it drops. |
| Daily humidity curve | Art | Perlin-noise painting that "breathes" over the day. |
| VOC pattern vs calendar | MachineTheatre | The HVAC and the fridge have a soap-opera "disagreement" over the kitchen's air. |
| Post-rain BME pressure | World | Your Stardew home logs rain; the virtual garden grows. |
| Pollen surrogate (PM + spectral) | Narrative | "Today is Chapter 7: The Plum Blossoms." Daily journal in a botanical-memoir style. |

---

## HR / HRV / bio (MAX30102, AD8232, GSR)

*(bio signals are most sensitive → creative software should always be opt-in on yourself, never others.)*

| Sensor event | Mode | Creative software response |
|---|---|---|
| HRV high-coherence window | Oracle | "You are tuned. Make the hard call in the next 20 min." |
| Skin-temp rising | Persona | A warm grandmother voice: "You're running hot, love. Water." |
| GSR spike during a specific calendar event | TimeShift | Play the HRV of your Last Good Run as proof the spike will pass. |
| Per-breath phase | Art | Drives the brightness of a hallway NeoPixel rope into a slow "room breathes with me." |
| Two people's HRV nearby | Social | If their HRV entrains (coherence > 0.5), the room plays a subtle chord. |
| Bored-with-desk IMU stillness + low HRV | Game | "Quest log: take 50 steps to refresh." |

---

## Door / magnetic / reed switch

| Sensor event | Mode | Creative software response |
|---|---|---|
| Door opens | Persona | Butler voice announces the date and a fact from history. |
| Door slams | Narrative | LLM appends a chapter break to the daily diary; new chapter title auto-generated. |
| Pattern of opens across a day | Art | Timelapse painting of the household's "breathing" in door-open pulses. |
| Door held open > 30 s | Environmental | HVAC politely retreats; a fake BBC voice: "I'll be going to sleep until you're finished." |
| Matched pair (two doors opened same minute) | Social | Your partner's keychain fob buzzes once. |

---

## Light / lux / spectral (TSL2591, AS7341)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Sunset detected | World | Your Minecraft/Stardew home's in-game sun sets too. |
| Monitor-glow vs room-light | Persona | A librarian voice: "You've gone screen-only again." |
| Daily total lux < 500 | Oracle | Tarot card: "The tower. Go outside." |
| Spectral shift when a bouquet is placed | Art | A daily watercolor updates, seeded by flower spectrum. |
| Candle lit (flicker FFT) | Narrative | Diary adds a "the candle was lit at 19:12" line. |

---

## GPS / pressure / dead-reckoning

| Sensor event | Mode | Creative software response |
|---|---|---|
| Altitude rising | Persona | A mountain-guide voice: "You'll want an extra layer." |
| First time visiting a GPS cell | World | Your personal "world map" pixel-art gains a new tile. |
| Daily km walked | Game | Earn "distance coins" spendable on e-paper-printed badges. |
| Unexpected route home | TimeShift | Device reads aloud the last time you took this route (last March; it was raining). |
| Floor change (BMP388 deltas) | Oracle | "You've ascended. The hour's meditation is patience." |

---

## LoRa packets (from field nodes, bee-hives, pets, drones)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Hive weight flat-lines 3 days | Persona | The hive sends a text in the voice of an elderly aunt. |
| Falcon telemetry shows stoop | Narrative | Glove e-paper whispers "she dove." |
| Shepherd's sheep motion unusual | MachineTheatre | Two sheep "gossip" about a third via LLM-scripted LoRa chat displayed at the farmhouse. |
| Drone ground-truth anchor pings | Game | Treasure hunt: each anchor is a riddle. |

---

## Capacitive touch array (cheap foil strips)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Sequence on 4 pads | Game | Opens a secret Slack channel at the office. |
| One pad held | Narrative | Audio clip starts — today's "episode" of a home-built podcast. |
| Two people touch simultaneously | Social | Their photo appears on a shared screen as the "duet of the day." |
| Random pad at random time | Oracle | Random pad = random color = random card-of-the-day. |

---

## Ultrasonic / distance (HC-SR04, VL53L1X)

| Sensor event | Mode | Creative software response |
|---|---|---|
| Close approach to a sculpture | Environmental | The sculpture's NeoPixel shifts color as you approach; it "notices" you. |
| Two passers-by in sequence | Art | A Rorschach blot generative-art builds, one blot per passer-by. |
| Distance held = stand-and-read | Persona | A reading-partner LLM offers to narrate the page. |

---

## Radar / weather

| Sensor event | Mode | Creative software response |
|---|---|---|
| Lightning strike within 10 km | Persona | A Viking voice: "Thor has spoken." |
| Barometric dip | Oracle | "A meeting will be moved; prepare." |
| Rain-onset pattern | World | The Minecraft/Stardew world rains. |
| Unexpected wind-shift | Narrative | The daily diary adds "the wind turned at 14:02, and so did the conversation." |

---

## Multi-signal fusions worth stealing

- **Air + mic + calendar** → a fake radio host narrates your cooking as a cook-off.
- **Door + HR** → detective-noir narration whenever you come home "at the end of another long day."
- **IMU (wrist) + mic** → "air-guitar mode": IMU strumming triggers a BLE-streamed power chord.
- **LoRa hive + weather forecast** → a fictional beekeeper-correspondent writes weekly letters to you.
- **CSI + room camera** → a "ghost cam" overlay where the CSI variance spawns a ghost dot on the live camera feed.
- **Thermal + mic** → a tiny creature-of-the-room lives where the hot spot is and squeaks when the mic peaks.

---

## Contribute your riffs

If you have an unusual software-layer for a hardware you've already got, add a row here. Include:

1. **Sensor event** (concrete, one line)
2. **Mode** (pick from the twelve)
3. **Creative software response** (one line; don't over-explain)

The gallery is the community's living imagination.

## See also

- [`software-as-imagination.md`](software-as-imagination.md) — the twelve modes in full
- [`information-asymmetry.md`](information-asymmetry.md) §"Two axes of asymmetry" — why meaning-asymmetry is unbounded
- [`../personas/11-creative-remix.md`](../personas/11-creative-remix.md) — ten personas where the entire asymmetry IS software imagination
- [`../projects/pirate-stage-walkby/`](../projects/pirate-stage-walkby/) — a complete build of one riff end-to-end
