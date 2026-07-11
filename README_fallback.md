# THX Card Fallback

Flash: [THXCard_fallback.uf2](/Users/adrianvos/coding/GitHub/XHT_card/uf2/THXCard_fallback.uf2)

This is the rollback-safe copy of the most recently accepted build.

Right now it matches the current passed main version, and it exists so future experiments can be tried without losing a known-good firmware.

Accepted behavior in this fallback:

- slower one-shot sweep, about 2.2 seconds for full travel
- `CV2` scaled so 0-5V controller/mod-wheel sources can cover the full note-position travel
- `CV Out 1` mirrors note position unless `CV1` is patched, then it becomes pitch
- `CV Out 2` always mirrors note position
- fallback reverb topology/gain, kept stable after testing
- `P1` gates the sound when patched; leave `P1` unpatched for sustained audio

Short control summary:

- `Main` — position / destination through the note
- `X` — delay
- `Y` — reverb
- switch up — octave up
- switch middle — normal octave
- momentary switch down — reset / one-shot from start to destination
- `CV1` in — pitch transpose
- `CV2` in — position and one-shot destination control
- `P1` in — gate when patched; sustained note when unpatched
- `P2` in — external stepped clock
- `Audio 1/2` — stereo output with effects
- `CV Out 1` — position, or pitch when `CV1` is patched
- `CV Out 2` — position
- `Pulse Out 1` — note gate
- `Pulse Out 2` — mirror of `P2`

Use this version if:

- you want the safety copy
- you want to compare against a new experiment
- you need a fast rollback after testing another build
