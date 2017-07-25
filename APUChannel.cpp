#include "APUChannel.h"
#include "APU.h"

APUChannel::APUChannel(const shared_ptr<APU> &apu, const shared_ptr<CPU> &cpu) {
    _apu = apu;
    _cpu = cpu;
}

APUChannel::~APUChannel(){

}
 
int APUChannel::tick(unsigned c) {
    APUChannel& ch = *this;
    if(!_apu->ChannelsEnabled[c]) return c==4 ? 64 : 8;
    int wl = (ch.reg.WaveLength+1) * (c >= 2 ? 1 : 2);
    if(c == 3) wl = _apu->NoisePeriods[ ch.reg.NoiseFreq ];
    int volume = ch.length_counter ? ch.reg.EnvDecayDisable ? ch.reg.FixedVolume : ch.envelope : 0;
    // Sample may change at wavelen intervals.
    auto& S = ch.level;
    if(!APU::count(ch.wave_counter, wl)) return S;
    switch(c)
    {
        default:// Square wave. With four different 8-step binary waveforms (32 bits of data total).
            if(wl < 8) return S = 8;
            return S = (0xF33C0C04u & (1u << (++ch.phase % 8 + ch.reg.DutyCycle * 8))) ? volume : 0;
        case 2: // Triangle wave
            if(ch.length_counter && ch.linear_counter && wl >= 3) ++ch.phase;
            return S = (ch.phase & 15) ^ ((ch.phase & 16) ? 15 : 0);
        case 3: // Noise: Linear feedback shift register
            if(!ch.hold) ch.hold = 1;
            ch.hold = (ch.hold >> 1)
                  | (((ch.hold ^ (ch.hold >> (ch.reg.NoiseType ? 6 : 1))) & 1) << 14);
            return S = (ch.hold & 1) ? 0 : volume;
        case 4: // Delta modulation channel (DMC)
            // hold = 8 bit value, phase = number of bits buffered
            if(ch.phase == 0) // Nothing in sample buffer?
            {
                if(!ch.length_counter && ch.reg.LoopEnabled) // Loop?
                {
                    ch.length_counter = ch.reg.PCMlength*16 + 1;
                    ch.address        = (ch.reg.reg0 | 0x300) << 6;
                }
                if(ch.length_counter > 0) // Load next 8 bits if available
                {
                    // Note: Re-entrant! But not recursive, because even
                    // the shortest wave length is greater than the read time.
                    // TODO: proper clock
                    if(ch.reg.WaveLength>20)
                        for(unsigned t=0; t<3; ++t) _cpu->read(u16(ch.address) | 0x8000); // timing
                    ch.hold  = _cpu->read(u16(ch.address++) | 0x8000); // Fetch byte
                    ch.phase = 8;
                    --ch.length_counter;
                }
                else // Otherwise, disable channel or issue IRQ
                    _apu->ChannelsEnabled[4] = ch.reg.IRQenable && (_cpu->intr = _apu->DMC_IRQ = true);
            }
            if(ch.phase != 0) // Update the signal if sample buffer nonempty
            {
                int v = ch.linear_counter;
                if(ch.hold & (0x80 >> --ch.phase)) v += 2; else v -= 2;
                if(v >= 0 && v <= 0x7F) ch.linear_counter = v;
            }
            return S = ch.linear_counter;
    }
}