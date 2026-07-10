# THX Card MIDI Experimental

Flash: [THXCardMidiExperimental.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCardMidiExperimental.uf2)

This is a separate experimental build. It does not replace the main card.

What it adds:

- MIDI note input for pitch transpose
- MIDI CC1 mapped to note position like `Main`
- smoothed MIDI CC1 response for more musical scrub control
- MIDI note output of the current chord snapshot, cleaned up for DAW recording

MIDI out gate behavior:

- if `P1` is not patched, MIDI out is always open
- if `P1` is patched, MIDI out follows the `P1` gate
- `P1` high sends the current chord
- `P1` low sends note-offs / silence

Current MIDI behavior:

- MIDI clock is not used
- USB product name is `THX Card MIDI`

Important limitation:

- the RP2040 USB setup here boots as either USB host or USB device, not both at the same time
- that means it can talk to a direct USB MIDI controller in host mode, or to a DAW/computer in device mode

Use this version if you want to explore MIDI behavior. It is build-tested, but it is still experimental and not yet promoted over the main build.
