# THX Card MIDI Fallback

Flash: [THXCardMidi_fallback.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCardMidi_fallback.uf2)

This is the most recent tested and accepted MIDI version.

Use this if a later MIDI build becomes unstable or if you want the known-good MIDI behavior from the hardware test pass.

Confirmed behavior:

- MIDI input from a DAW changes pitch
- USB MIDI controller input works when the card boots in host mode
- MIDI CC1/mod wheel controls note movement
- MIDI note-on opens the sound
- MIDI note-off closes the sound unless sustain is held
- sustain pedal is `CC64`
- MIDI clock is not used
- MIDI out records useful chord notes to a DAW
- if `P1` is patched, `P1` gates MIDI out
- if `P1` is not patched, MIDI out is not gated
- USB product name is `THX Card MIDI`
- USB host/device role detection follows the same boot pattern as CozmikC1zzl3

Short control summary:

- `Main` — note position / one-shot destination unless recent `CC1` is taking over destination
- `X` — delay
- `Y` — reverb
- switch up — octave up
- switch middle — normal octave
- momentary switch down — reset / one-shot from start to destination
- `CV1` in — pitch transpose
- `CV2` in — position and one-shot destination control
- `P1` in — audio gate when patched, and MIDI-out gate when patched
- `P2` in — external stepped clock
- MIDI note in — pitch transpose and note gate
- MIDI `CC1` / mod wheel — note destination
- MIDI `CC64` — sustain pedal
- MIDI clock — ignored

Note: the active [THXCardMidi.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCardMidi.uf2) build adds the LED 5 USB role diagnostic. This fallback remains the frozen hardware-passed MIDI rollback copy.

Important USB note:

- the card boots as either USB host or USB device, not both
- for direct USB controller testing, connect the controller first, then power the card
- pressing reset alone may not force USB role renegotiation

The non-MIDI fallback remains separate:

[THXCard_fallback.uf2](/Users/adrianvos/coding/GitHub/XHT_card/THXCard_fallback.uf2)
