# THX Card

This folder now contains three user-facing firmware variants for the Music Thing Modular Workshop Computer.

- `THXCard.uf2` — current main build
- `THXCard_fallback.uf2` — last accepted rollback-safe build
- `THXCardMidiExperimental.uf2` — tested MIDI build

If you just want the current recommended build, flash `THXCard.uf2`.

## Quick Guide

- [README_main.md](/Users/adrianvos/coding/GitHub/XHT_card/README_main.md) — current recommended build
- [README_fallback.md](/Users/adrianvos/coding/GitHub/XHT_card/README_fallback.md) — rollback-safe build
- [README_midi_experimental.md](/Users/adrianvos/coding/GitHub/XHT_card/README_midi_experimental.md) — tested MIDI build

## Notes

- The sound is a synthesised deep-note-style gesture, not a bundled THX sample.
- The project runs at `192 MHz`.
- The build uses the current authoritative `ComputerCard.h` from the Workshop Computer tree.
- The synthesis idea was inspired by Tod E. Kurt's GPL-3.0 `derpnote2`, so this project remains GPL-3.0.
- In the MIDI build, `P1` only gates MIDI out when `P1` is physically patched. Unpatched `P1` leaves MIDI out always open.
- In the MIDI build, note-on/note-off and sustain pedal (`CC64`) now control whether the sound stays open when playing from a keyboard.
- In current active builds, `CV Out 1` mirrors note position unless `CV1` is patched, when it becomes pitch; `CV Out 2` always mirrors note position.
- In current active builds, analog `CV2` is scaled so 0-5V controller/mod-wheel sources can cover the full note-position travel.
- Pulse Out 1 mirrors note gate behavior and Pulse Out 2 mirrors the `P2` input pulse.
