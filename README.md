# THX Card

A playable stereo deep-note-inspired swarm for the Music Thing Modular Workshop Computer.
It synthesises the gesture (sixteen detuned saw voices converging on a wide chord) rather
than bundling the copyrighted THX recording.

## Controls

- **Main** — scrub through the convergence; with P2 patched, P2 clocks sixteen positions
- **CV1** — quantised pitch transpose; **CV2** — position offset
- **P1** — unpatched sustains; patched follows the incoming gate length
- **P2** — external position clock and mirrored trigger output
- **X / Y** — delay time / reverb amount
- **Switch up / middle / down** — octave up / normal / reset
- **Audio 1/2 in** — stereo external audio mixed through the effects
- **Audio 1/2 out** — effected stereo mix
- **CV 1/2 out** — played pitch, upper/lower octave

The firmware runs at **192 MHz**, uses integer DSP, contains no division in the per-sample
path, and is copied completely to RAM. It builds against the current authoritative
`ComputerCard.h` in `Workshop_Computer/Demonstrations+HelloWorlds/PicoSDK/ComputerCard`.

## Build

```sh
cmake -S . -B build -DPICO_NO_PICOTOOL=1
cmake --build build -j2
```

The synthesis structure was inspired by Tod E. Kurt's GPL-3.0 `derpnote2` sketch; this
project is therefore distributed under GPL-3.0.
