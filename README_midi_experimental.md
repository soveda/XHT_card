# THX Card MIDI Experimental

Flash: [THXCardMidiExperimental.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCardMidiExperimental.uf2)

This is a separate experimental build. It does not replace the main card.

What it adds:

- MIDI note input for pitch transpose
- MIDI CC1 sets a destination for musical note movement
- MIDI note output of the current chord snapshot, cleaned up for DAW recording

MIDI note gate behavior:

- MIDI note-on opens the sound
- MIDI note-off closes the sound
- sustain pedal (`CC64`) holds the sound after note-off
- if sustain is released with no held notes, the sound closes
- before any MIDI note activity, the card still behaves normally without requiring MIDI gating

MIDI out gate behavior:

- if `P1` is not patched, MIDI out is always open
- if `P1` is patched, MIDI out follows the `P1` gate
- `P1` high sends the current chord
- `P1` low sends note-offs / silence

Current MIDI behavior:

- MIDI clock is not used
- USB product name is `THX Card MIDI`
- audible sound is gated by both MIDI note state and `P1` if `P1` is patched
- analog `CV2` is scaled for full position travel from 0-5V controller/mod-wheel sources
- `CV Out 1` mirrors note position unless `CV1` is patched, then it becomes pitch
- `CV Out 2` always mirrors the current note-position state
- `Pulse Out 1` mirrors the current note gate behavior
- `Pulse Out 2` mirrors the incoming `P2` pulse

Important limitation:

- the RP2040 USB setup here boots as either USB host or USB device, not both at the same time
- that means it can talk to a direct USB MIDI controller in host mode, or to a DAW/computer in device mode

Use this version if you want the tested MIDI build. It remains separate from the main card so the non-MIDI fallback stays simple and safe.
