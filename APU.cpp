#include "APU.h"
#include <iostream>

const u8 APU::m_LengthCounters[32] = { 10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
                                      12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30 };
const u16 APU::m_noisePeriods[16] = { 2,4,8,16,32,48,64,80,101,127,190,254,381,508,1017,2034 };
const u16 APU::m_DMCperiods[16] = { 428,380,340,320,286,254,226,214,190,160,142,128,106,84,72,54 };                               
const long APU::m_sampleRate = 96000;    // 44.1 kHz sample rate
const double APU::m_clockRate = 1789773; // 1.7 MHz clock rate
const int APU::m_frameRate = 60;         // 60 frames of sound per second
const int APU::m_cyclesPerFrame = APU::m_clockRate / APU::m_frameRate;

Sync_Audio APU::m_audio;
Blip_Buffer APU::m_blipBuffer;
Blip_Synth<blip_good_quality,15> APU::m_synthSquare1;
Blip_Synth<blip_good_quality,15> APU::m_synthSquare2;
Blip_Synth<blip_good_quality,15> APU::m_synthTriangle;
Blip_Synth<blip_med_quality,15> APU::m_synthNoise;
Blip_Synth<blip_med_quality,127> APU::m_synthDmc;

APU::APU(const std::shared_ptr<CPU> &cpu) : m_channels { APUChannel(cpu),
    APUChannel(cpu),
    APUChannel(cpu),
    APUChannel(cpu),
    APUChannel(cpu) } { 
    _cpu = cpu;
    m_currentSample = 0;
}

APU::~APU() {
    m_audio.stop();	
}

void APU::init() {
    // Setup buffer
	m_blipBuffer.clock_rate(m_clockRate);
	m_blipBuffer.set_sample_rate(m_sampleRate, 1000 / m_frameRate);	
	
	// Setup synth
	m_synthSquare1.volume(0.50);
	m_synthSquare1.output(&m_blipBuffer);

    m_synthSquare2.volume(0.50);
	m_synthSquare2.output(&m_blipBuffer);

    m_synthTriangle.volume(0.50);
	m_synthTriangle.output(&m_blipBuffer);

    m_synthNoise.volume(0.50);
	m_synthNoise.output(&m_blipBuffer);

    m_synthDmc.volume(0.50);
	m_synthDmc.output(&m_blipBuffer);
	
	// Start audio
	m_audio.start(m_sampleRate);
}

inline bool count(int& value, int reset) { 
    return --value < 0 ? (value=reset) , true : false; 
}

void APU::write(u8 index, u8 value) {
    APUChannel& ch = m_channels[(index/4) % 5];
    switch (index<0x10 ? index%4 : index) {
        case 0: 
            if (ch.m_reg.LinearCounterDisable) {
                ch.linearCounter(value & 0x7F); 
            }
            ch.m_reg.reg0 = value; 
            break;
        case 1: 
            ch.m_reg.reg1 = value; 
            ch.sweepDelay(ch.m_reg.SweepRate); 
            break;
        case 2: ch.m_reg.reg2 = value; break;
        case 3:
            ch.m_reg.reg3 = value;
            if (m_channelsEnabled[index/4]) {
                ch.lengthCounter(m_LengthCounters[ch.m_reg.LengthCounterInit]);
            }
            ch.linearCounter(ch.m_reg.LinearCounterInit);
            ch.envDelay(ch.m_reg.EnvDecayRate);
            ch.envelope(15);
            if(index < 8) {
                ch.phase(0);
            }
            break;
        case 0x10: 
            ch.m_reg.reg3 = value; 
            ch.m_reg.WaveLength = m_DMCperiods[value&0x0F]; 
            break;
        case 0x12: 
            ch.m_reg.reg0 = value; 
            ch.address((ch.m_reg.reg0 | 0x300) << 6); 
            break;
        case 0x13: 
            ch.m_reg.reg1 = value; 
            ch.lengthCounter(ch.m_reg.PCMlength * 16 + 1); 
            break; // sample length
        case 0x11: ch.linearCounter(value & 0x7F); break; // dac value
        case 0x15: //channel enabler
            for (unsigned c=0; c<5; ++c) {
                m_channelsEnabled[c] = value & (1 << c);
            }
            for (unsigned c=0; c<5; ++c) {
                if (!m_channelsEnabled[c]) {
                    m_channels[c].lengthCounter(0);
                } else if (c == 4 && m_channels[c].lengthCounter()) {
                    m_channels[c].lengthCounter(ch.m_reg.PCMlength * 16 + 1);
                }
            }
            break;
        case 0x17:
            m_IRQdisable       = value & 0x40;
            m_fiveCycleDivider = value & 0x80;
            m_hz240counter     = { 0,0 };
            if (m_IRQdisable) {
                m_periodicIRQ = m_DMC_IRQ = false;
            }
    }
}

u8 APU::read() {
    u8 result = 0;
    for (unsigned c=0; c<5; ++c) {
        result |= (m_channels[c].lengthCounter() ? 1 << c : 0);
    }
    if (m_periodicIRQ) {
        result |= 0x40; 
    } 
    m_periodicIRQ = false;
    if (m_DMC_IRQ) {
        result |= 0x80;
    }     
    m_DMC_IRQ = false;
    _cpu->intr = false;
    return result;
}

void APU::tick() { // Invoked at CPU's rate. 
    // Divide CPU clock by 7457.5 to get a 240 Hz, which controls certain events.
    if((m_hz240counter.lo += 2) >= 14915) {
        m_hz240counter.lo -= 14915;
        if(++m_hz240counter.hi >= 4+m_fiveCycleDivider) {
            m_hz240counter.hi = 0;
        }
        // 60 Hz interval: IRQ. IRQ is not invoked in five-cycle mode (48 Hz).
        if(!m_IRQdisable && !m_fiveCycleDivider && m_hz240counter.hi==0){
            _cpu->intr = m_periodicIRQ = true;
        }
        // Some events are invoked at 96 Hz or 120 Hz rate. Others, 192 Hz or 240 Hz.
        bool halfTick = (m_hz240counter.hi&5)==1, fullTick = m_hz240counter.hi < 4;
		unsigned channelNumber = 0;
        for(APUChannel& channel : m_channels) {            
            int wl = channel.m_reg.WaveLength;
            // Length tick (all channels except DMC, but different disable bit for triangle wave)
			if (halfTick && channel.lengthCounter() && 
                !(channelNumber == 2 ? (bool)channel.m_reg.LinearCounterDisable : (bool)channel.m_reg.LengthCounterDisable)) {
				channel.lengthCounter(channel.lengthCounter() - 1);
			}
            // Sweep tick (square waves only)
            if (halfTick && channelNumber < 2) {
                int sweepDelay = channel.sweepDelay();
                bool countResult = count(sweepDelay, channel.m_reg.SweepRate);
                channel.sweepDelay(sweepDelay);

                if(countResult && wl >= 8 && channel.m_reg.SweepEnable && channel.m_reg.SweepShift) {
                    int s = wl >> channel.m_reg.SweepShift, d[4] = {s, s, ~s, -s};
                    wl += d[channel.m_reg.SweepDecrease*2 + channelNumber];
					if (wl < 0x800) {
						channel.m_reg.WaveLength = wl;
					}
                }
            }
            // Linear tick (triangle wave only)
            if (fullTick && channelNumber == 2) {
				channel.linearCounter(channel.m_reg.LinearCounterDisable ?
                                      channel.m_reg.LinearCounterInit : 
                                      (channel.linearCounter() > 0 ? channel.linearCounter() - 1 : 0));
            }
            // Envelope tick (square and noise channels)            
            if (fullTick && channelNumber != 2) {
                int envDelay = channel.envDelay();
                bool countResult = count(envDelay, channel.m_reg.EnvDecayRate);
                channel.envDelay(envDelay);
                if (countResult && ((channel.envelope() > 0) || channel.m_reg.EnvDecayLoopEnable)) {
					channel.envelope((channel.envelope()-1) & 15);
                }
            }			
			channelNumber++;
        }
    }

    //synthesize all channels
	if (m_currentSample <= m_cyclesPerFrame) {	
		m_synthSquare1.update(m_currentSample, channelTick(0));
		m_synthSquare2.update(m_currentSample, channelTick(1));
		m_synthTriangle.update(m_currentSample, channelTick(2));
		m_synthNoise.update(m_currentSample, channelTick(3));
		m_synthDmc.update(m_currentSample, channelTick(4));

        m_currentSample++;        
    } 
	
    if (m_currentSample == m_cyclesPerFrame) {        
        m_blipBuffer.end_frame(m_cyclesPerFrame);
        long count = m_blipBuffer.read_samples(m_samples, m_BufferSize);
        m_audio.write(m_samples, count);
        m_currentSample = 0;
    }
}

int APU::channelTick(unsigned channelNumber) {
	return m_channels[channelNumber].tick(channelNumber == 1 ? 0 : channelNumber, m_channelsEnabled, m_noisePeriods, m_DMC_IRQ);
}