# AuraLens companion — first-boot phone setup

The first time the badge boots it can raise a SoftAP (`AuraLens-XXXX`, open) and serve a single page at `192.168.4.1`. Connect, pick 6 interest tags from the list, tap **Save**, and the ESP32 writes a 32-bit hash to NVS.

After that the badge is fully offline.

## Architecture

```
[phone]  --(SoftAP Wi-Fi)--> [ ESP32-C3 serves / via HTTP] --(NVS write)--> flash
```

## Minimal sample

```html
<!doctype html>
<meta name=viewport content="width=device-width,initial-scale=1">
<title>AuraLens setup</title>
<style>
  body{font:16px/1.4 system-ui;margin:2em;max-width:26em}
  label{display:block;margin:.4em 0}
  button{padding:.6em 1em;border-radius:.3em;border:0;background:#222;color:#fff}
</style>
<h1>Pick 6 of your interests</h1>
<form action="/set" method="POST">
  <input name=name placeholder="Your name" required>
  <!-- 32 checkboxes, one per interest bit -->
  <label><input type=checkbox name=b value=0> Sci-fi</label>
  <label><input type=checkbox name=b value=1> ESP32</label>
  <label><input type=checkbox name=b value=2> Westerns</label>
  <!-- … 29 more … -->
  <button type=submit>Save</button>
</form>
```

The ESP32 reads POST data (up to 6 bits set), ORs them into `interestVector`, `prefs.putUInt("vec", v)`, reboots into advertise mode.

## Extension ideas

- Progressive Web App that remembers your tags (for re-programming different badges).
- A public list of tag → emoji → color mapping on a community page so everyone's "Westerns" pulses the same color.
