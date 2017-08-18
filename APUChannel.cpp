#include "APUChannel.h"
#include "APU.h"

APUChannel::APUChannel(const shared_ptr<APU> &apu, const shared_ptr<CPU> &cpu) {
    _apu = apu;
    _cpu = cpu;

    m_lengthCounter = 0;
    m_linearCounter = 0;
    m_address = 0;
    m_envelope = 0;
    m_sweepDelay = 0;
    m_envDelay = 0; 
    wave_counter = 0;
    hold = 0;
    phase = 0;
    level = 0;
}

APUChannel::~APUChannel(){

}

const int& APUChannel::lengthCounter() const { 
    return m_lengthCounter; 
}

void APUChannel::lengthCounter(const int& lengthCounter) { 
    m_lengthCounter = lengthCounter; 
} 

const int& APUChannel::linearCounter() const { 
    return m_linearCounter; 
}

void APUChannel::linearCounter(const int& linearCounter) { 
    m_linearCounter = linearCounter; 
} 

const int& APUChannel::address() const { 
    return m_address; 
}

void APUChannel::address(const int& address) { 
    m_address = address; 
} 

const int& APUChannel::envelope() const { 
    return m_envelope; 
}

void APUChannel::envelope(const int& envelope) { 
    m_envelope = envelope; 
} 

const int& APUChannel::sweepDelay() const { 
    return m_sweepDelay; 
}

void APUChannel::sweepDelay(const int& sweepDelay) { 
    m_sweepDelay = sweepDelay; 
} 

const int& APUChannel::envDelay() const { 
    return m_envDelay; 
}

void APUChannel::envDelay(const int& envDelay) { 
    m_envDelay = envDelay; 
} 

inline bool count(int& value, int reset) { 
    return --value < 0 ? (value=reset) , true : false; 
}
 
int APUChannel::tick(unsigned channelNumber, bool channelsEnabled[], const u16 noisePeriods[], bool &dmcIrq) {
    APUChannel& ch = *this;
    if (!channelsEnabled[channelNumber]) {
        return channelNumber == 4 ? 64 : 8;
    }
    int wl = (ch.reg.WaveLength + 1) * (channelNumber >= 2 ? 1 : 2);
    if (channelNumber == 3) {
        wl = noisePeriods[ch.reg.NoiseFreq];
    }
    int volume = m_lengthCounter ? ch.reg.EnvDecayDisable ? ch.reg.FixedVolume : m_envelope : 0;
    // Sample may change at wavelen intervals.
    auto& sample = ch.level;
    if(!count(ch.wave_counter, wl)) {
        return sample;
    }

    switch(channelNumber) {
        default:// Square wave. With four different 8-step binary waveforms (32 bits of data total).
            if (wl < 8) {
                return sample = 8;
            }
            return sample = (0xF33C0C04u & (1u << (++ch.phase % 8 + ch.reg.DutyCycle * 8))) ? volume : 0;
        case 2: // Triangle wave
            if (m_lengthCounter && m_linearCounter && wl >= 3) {
                ++ch.phase;
            }
            return sample = (ch.phase & 15) ^ ((ch.phase & 16) ? 15 : 0);
        case 3: // Noise: Linear feedback shift register
            if (!ch.hold) {
                ch.hold = 1;
            }
            ch.hold = (ch.hold >> 1)
                  | (((ch.hold ^ (ch.hold >> (ch.reg.NoiseType ? 6 : 1))) & 1) << 14);
            return sample = (ch.hold & 1) ? 0 : volume;
        case 4: // Delta modulation channel (DMC)
            // hold = 8 bit value, phase = number of bits buffered
            if (ch.phase == 0) // Nothing in sample buffer?
            {
                if(!m_lengthCounter && ch.reg.LoopEnabled) // Loop?
                {
                    m_lengthCounter = ch.reg.PCMlength * 16 + 1;
                    m_address       = (ch.reg.reg0 | 0x300) << 6;
                }
                if(m_lengthCounter > 0) // Load next 8 bits if available
                {
                    // Note: Re-entrant! But not recursive, because even
                    // the shortest wave length is greater than the read time.
                    // TODO: proper clock
                    if(ch.reg.WaveLength>20)
                        for(unsigned t=0; t<3; ++t) _cpu->read(u16(m_address) | 0x8000); // timing
                    ch.hold  = _cpu->read(u16(m_address++) | 0x8000); // Fetch byte
                    ch.phase = 8;
                    m_lengthCounter--;
                }
                else // Otherwise, disable channel or issue IRQ
					channelsEnabled[4] = ch.reg.IRQenable && (_cpu->intr = dmcIrq = true);
            }
            if(ch.phase != 0) // Update the signal if sample buffer nonempty
            {
                int linearCounter = m_linearCounter;
                if(ch.hold & (0x80 >> --ch.phase)) {
                    linearCounter += 2; 
                } else {
                    linearCounter -= 2;
                }
                if(linearCounter >= 0 && linearCounter <= 0x7F) {
                    m_linearCounter = linearCounter;
                }
            }
            return sample = m_linearCounter;
    }
}