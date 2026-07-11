# THX Card Fallback

Flash: [THXCard_fallback.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCard_fallback.uf2)

This is the rollback-safe copy of the most recently accepted build.

Right now it matches the current passed main version, and it exists so future experiments can be tried without losing a known-good firmware.

Accepted behavior in this fallback:

- slower one-shot sweep, about 2.2 seconds for full travel
- `CV2` scaled so 0-5V controller/mod-wheel sources can cover the full note-position travel
- `CV Out 1` mirrors note position unless `CV1` is patched, then it becomes pitch
- `CV Out 2` always mirrors note position
- fallback reverb topology/gain, kept stable after testing
- `P1` gates the sound when patched; leave `P1` unpatched for sustained audio

Use this version if:

- you want the safety copy
- you want to compare against a new experiment
- you need a fast rollback after testing another build
