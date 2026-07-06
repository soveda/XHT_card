#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "ComputerCard.h"

namespace {
constexpr uint32_t kControlMask = 31;       // controls at 1.5 kHz
constexpr int32_t kVoices = 16;
constexpr int32_t kDelaySize = 16384;       // 341 ms maximum at 48 kHz
constexpr int32_t kReverbSize = 4096;
constexpr int32_t kChord[kVoices] = {26,38,50,57,62,69,74,81,62,69,74,81,86,86,90,90};
constexpr int32_t kStartNotes[kVoices] = {45,52,43,50,47,54,41,56,44,58,39,60,46,63,42,65};
constexpr uint32_t kDriftSeed[kVoices] = {
    0x0a65f2d1u, 0x31b4c9f5u, 0x7e4291b3u, 0x19ef6ac7u,
    0x5d34b271u, 0x24c7dd89u, 0x6b12a45fu, 0x3f98d2b5u,
    0x11d76e43u, 0x72ab094du, 0x28cf5179u, 0x4cd8e36bu,
    0x63f19427u, 0x16ac7f91u, 0x55be2083u, 0x2bd963ddu
};

constexpr uint32_t kSemitoneRatioQ30 = 1137589835u; // 2^(1/12) * 2^30
constexpr uint32_t kA4Increment = 39370534u;        // 440 * 2^32 / 48000

constexpr uint32_t mulQ30(uint32_t a, uint32_t b) {
    return uint32_t((uint64_t(a) * b) >> 30);
}

uint32_t __not_in_flash_func(noteIncrement)(int32_t note) {
    note = note < 0 ? 0 : (note > 127 ? 127 : note);
    uint32_t inc = kA4Increment;
    if (note >= 69) {
        for (int32_t n = 69; n < note; ++n) inc = mulQ30(inc, kSemitoneRatioQ30);
    } else {
        for (int32_t n = note; n < 69; ++n) inc = uint32_t((uint64_t(inc) << 30) / kSemitoneRatioQ30);
    }
    return inc;
}

inline int32_t __not_in_flash_func(saw)(uint32_t phase) {
    return int32_t(phase >> 20) - 2048;
}

inline int32_t __not_in_flash_func(clamp12)(int32_t x) {
    return x < -2048 ? -2048 : (x > 2047 ? 2047 : x);
}

inline uint32_t __not_in_flash_func(lcgStep)(uint32_t value) {
    return value * 1664525u + 1013904223u;
}
}

class THXCardDense final : public ComputerCard {
public:
    THXCardDense() {
        for (int32_t i = 0; i < kVoices; ++i) {
            phase_[i] = 0x9e3779b9u * uint32_t(i + 1);
            driftState_[i] = kDriftSeed[i];
            startInc_[i] = noteIncrement(kStartNotes[i]);
            targetInc_[i] = noteIncrement(kChord[i]);
            glideShape_[i] = uint16_t(32768u + uint32_t(i * 2048));
        }
    }

    void __not_in_flash_func(ProcessSample)() override {
        const bool p1 = PulseIn1();
        const bool p2 = PulseIn2();

        if ((sampleCounter_++ & kControlMask) == 0) updateControls(p2);
        if (resetRequested_) resetNote();

        const bool audible = !p1Connected_ || p1;
        envelope_ += ((audible ? 32767 : 0) - envelope_) >> 7;

        int32_t left = 0;
        int32_t right = 0;
        const uint32_t position = position_;
        const uint32_t pitchRatio = pitchRatioQ16_;
        const uint32_t density = densityQ16_;
        for (int32_t i = 0; i < kVoices; ++i) {
            const uint32_t voiceProgress = shapedProgress(position, glideShape_[i]);
            const int64_t delta = int64_t(targetInc_[i]) - int64_t(startInc_[i]);
            int32_t inc = int32_t(int64_t(startInc_[i]) + ((delta * voiceProgress) >> 16));

            const int32_t residual = int32_t((delta * (65535u - density)) >> 16);
            driftState_[i] = lcgStep(driftState_[i]);
            const int32_t jitter = int32_t((driftState_[i] >> 20) & 0x7ff) - 1024;
            inc += (residual * jitter) >> 13;

            inc = int32_t((int64_t(inc) * pitchRatio) >> 16);
            phase_[i] += uint32_t(inc);

            int32_t voice = saw(phase_[i]);
            const int32_t blend = int32_t((uint32_t(jitter + 1024) * (65535u - density)) >> 16);
            voice = (voice * (2048 - (blend >> 1))) >> 11;

            if (i & 1) right += voice;
            else left += voice;
        }

        left = (left * envelope_) >> 18;
        right = (right * envelope_) >> 18;

        left += AudioIn1() >> 1;
        right += AudioIn2() >> 1;
        processEffects(left, right);

        AudioOut1(int16_t(clamp12(left)));
        AudioOut2(int16_t(clamp12(right)));
        PulseOut1(p1Connected_ ? p1 : audible);
        PulseOut2(p2);
    }

private:
    void __not_in_flash_func(updateControls)(bool p2) {
        const int32_t main = KnobVal(Knob::Main);
        const int32_t cvPos = CVIn2();
        int32_t pos = (main << 4) + (cvPos << 4);

        p1Connected_ = Connected(Input::Pulse1);
        const bool p2Connected = Connected(Input::Pulse2);
        if (p2Connected) {
            if (p2 && !lastP2_) clockPosition_ += 4096;
            pos = int32_t(clockPosition_);
        }
        lastP2_ = p2;
        position_ = uint32_t(pos < 0 ? 0 : (pos > 65535 ? 65535 : pos));

        const int32_t semitones = (CVIn1() * 48) >> 11;
        pitchMillivolts_ = (semitones * 1000) / 12;
        pitchRatioQ16_ = ratioForSemitones(semitones);

        const int32_t xVal = KnobVal(Knob::X);
        if (!switchDownHeld_) delaySamples_ = 64 + ((xVal * (kDelaySize - 65)) >> 12);
        densityQ16_ = uint32_t(KnobVal(Knob::Y)) << 4;

        const Switch sw = SwitchVal();
        switchDownHeld_ = sw == Switch::Down;
        octaveOffset_ = sw == Switch::Up ? 12 : 0;
        if (switchDownHeld_) reverbAmount_ = 2048 + (xVal << 3);
        if (switchDownHeld_ && !lastSwitchDown_) resetRequested_ = true;
        lastSwitchDown_ = switchDownHeld_;
        if (octaveOffset_) pitchRatioQ16_ <<= 1;

        CVOut1Millivolts(pitchMillivolts_ + 1000);
        CVOut2Millivolts(pitchMillivolts_);

        LedBrightness(0, uint16_t(position_ >> 4));
        LedBrightness(1, uint16_t(KnobVal(Knob::X)));
        LedBrightness(2, uint16_t(envelope_ >> 3));
        LedBrightness(3, uint16_t(KnobVal(Knob::Y)));
        LedOn(4, PulseIn1());
        LedOn(5, p2);
    }

    static uint32_t __not_in_flash_func(ratioForSemitones)(int32_t semitones) {
        uint32_t ratio = 65536;
        if (semitones >= 0) {
            for (int32_t i = 0; i < semitones; ++i) ratio = uint32_t((uint64_t(ratio) * kSemitoneRatioQ30) >> 30);
        } else {
            for (int32_t i = semitones; i < 0; ++i) ratio = uint32_t((uint64_t(ratio) << 30) / kSemitoneRatioQ30);
        }
        return ratio;
    }

    static uint32_t __not_in_flash_func(shapedProgress)(uint32_t position, uint16_t glideShape) {
        const uint32_t curved = uint32_t((uint64_t(position) * position) >> 16);
        return uint32_t(((uint64_t(position) * (65535u - glideShape)) + ((uint64_t(curved) * glideShape) >> 16)) >> 16);
    }

    void __not_in_flash_func(processEffects)(int32_t &left, int32_t &right) {
        const uint32_t read = (delayWrite_ - uint32_t(delaySamples_)) & (kDelaySize - 1);
        const int32_t dl = delayL_[read];
        const int32_t dr = delayR_[read];
        delayL_[delayWrite_] = int16_t(clamp12(left + (dr >> 1)));
        delayR_[delayWrite_] = int16_t(clamp12(right + (dl >> 1)));
        delayWrite_ = (delayWrite_ + 1) & (kDelaySize - 1);
        left += dl >> 1;
        right += dr >> 1;

        const uint32_t rr = (reverbWrite_ - 3067u) & (kReverbSize - 1);
        const int32_t wet = reverb_[rr];
        const int32_t mono = (left + right) >> 1;
        reverb_[reverbWrite_] = int16_t(clamp12(mono + ((wet * 3) >> 2)));
        reverbWrite_ = (reverbWrite_ + 1) & (kReverbSize - 1);
        left += (wet * reverbAmount_) >> 15;
        right -= (wet * reverbAmount_) >> 15;
    }

    void __not_in_flash_func(resetNote)() {
        position_ = 0;
        clockPosition_ = 0;
        resetRequested_ = false;
    }

    uint32_t phase_[kVoices]{};
    uint32_t driftState_[kVoices]{};
    uint32_t startInc_[kVoices]{};
    uint32_t targetInc_[kVoices]{};
    uint16_t glideShape_[kVoices]{};
    int16_t delayL_[kDelaySize]{};
    int16_t delayR_[kDelaySize]{};
    int16_t reverb_[kReverbSize]{};
    uint32_t delayWrite_ = 0, reverbWrite_ = 0, sampleCounter_ = 0;
    uint32_t position_ = 0, clockPosition_ = 0, pitchRatioQ16_ = 65536, densityQ16_ = 65535;
    int32_t envelope_ = 32767, delaySamples_ = 4000, reverbAmount_ = 12288;
    int32_t pitchMillivolts_ = 0, octaveOffset_ = 0;
    bool p1Connected_ = false, lastP2_ = false, resetRequested_ = false;
    bool switchDownHeld_ = false, lastSwitchDown_ = false;
};

int main() {
    set_sys_clock_khz(192000, true);
    static THXCardDense card;
    card.EnableNormalisationProbe();
    card.Run();
}
