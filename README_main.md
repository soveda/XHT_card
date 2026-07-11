# THX Card Main

Flash: [THXCard.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCard.uf2)

This is the current recommended version.

What it does:

- creates a stereo deep-note-style chord swarm
- `Main` scrubs through the note
- pressing reset launches a one-shot from the start toward the current destination
- `CV2` can move the destination across the full note travel, including 0-5V controller/mod-wheel sources
- `P2` clocks the note through stepped positions and overrides the one-shot
- `CV Out 1` mirrors note position unless `CV1` is patched; then it outputs pitch
- `CV Out 2` always mirrors the current note-position state

Controls:

- `Main` — position / destination through the note
- `CV1` — pitch transpose
- `CV2` — position offset and one-shot destination contribution, scaled for full travel from 0-5V sources
- `P1` — gate the sound when patched, sustain when unpatched
- `P2` — external clock for stepped note position
- `X` — delay
- `Y` — reverb
- `Switch Up` — octave up
- `Switch Middle` — normal
- `Switch Down` — reset

Outputs:

- `Audio Out 1/2` — stereo audio with delay and reverb
- `CV Out 1` — current note-position state, or pitch when `CV1` is patched
- `CV Out 2` — current note-position state as CV
- `Pulse Out 1` — note gate state; with `P1` patched it mirrors `P1`, unpatched it stays high while the note is audible
- `Pulse Out 2` — direct mirror of `P2` input

Use this version if you want the tested, accepted build.
