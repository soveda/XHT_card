# THX Card Main

Flash: [THXCard.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCard.uf2)

This is the current recommended version.

What it does:

- creates a stereo deep-note-style chord swarm
- `Main` scrubs through the note
- pressing reset launches a one-shot from the start toward the current destination
- `CV2` can move the destination across the full note travel
- `P2` clocks the note through stepped positions and overrides the one-shot
- `CV Out 1` and `CV Out 2` both mirror the current note-position state
- active builds now use a lightweight multi-stage reverb for a rounder tail than the older fallback build

Controls:

- `Main` — position / destination through the note
- `CV1` — pitch transpose
- `CV2` — position offset and one-shot destination contribution
- `P1` — gate the sound when patched, sustain when unpatched
- `P2` — external clock for stepped note position
- `X` — delay
- `Y` — reverb
- `Switch Up` — octave up
- `Switch Middle` — normal
- `Switch Down` — reset

Outputs:

- `Audio Out 1/2` — stereo audio with delay and reverb
- `CV Out 1` — current note-position state as CV
- `CV Out 2` — current note-position state as CV
- `Pulse Out 1` — note gate state; with `P1` patched it mirrors `P1`, unpatched it stays high while the note is audible
- `Pulse Out 2` — direct mirror of `P2` input

Use this version if you want the tested, accepted build.
