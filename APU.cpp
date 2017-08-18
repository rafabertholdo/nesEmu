#include "APU.h"
#include <iostream>

const u8 APU::_lengthCounters[32] = { 10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
                               12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30 };

const u16 APU::_noisePeriods[16] = { 2,4,8,16,32,48,64,80,101,127,190,254,381,508,1017,2034 };
const u16 APU::_DMCperiods[16] = { 428,380,340,320,286,254,226,214,190,160,142,128,106,84,72,54 };                               
const long APU::_sampleRate = 96000;    // 44.1 kHz sample rate
const double APU::_clockRate = 1789773; // 1 MHz clock rate
const int APU::_frameRate = 60;         // 60 frames of sound per second
const int APU::_cyclesPerFrame = APU::_clockRate / APU::_frameRate;
Sync_Audio APU::_audio;
Blip_Buffer APU::_blipBuffer;
Blip_Synth<blip_good_quality,15> APU::_synthSquare1;
Blip_Synth<blip_good_quality,15> APU::_synthSquare2;
Blip_Synth<blip_good_quality,15> APU::_synthTriangle;
Blip_Synth<blip_med_quality,15> APU::_synthNoise;
Blip_Synth<blip_med_quality,127> APU::_synthDmc;

APU::APU(const std::shared_ptr<CPU> &cpu) : _channels { APUChannel(nullptr, cpu) ,
	APUChannel(nullptr, cpu)  ,
	APUChannel(nullptr, cpu)  ,
	APUChannel(nullptr, cpu)  ,
	APUChannel(nullptr, cpu) } { 
    _cpu = cpu;
    _currentSample = 0;
}

APU::~APU() {
    _audio.stop();	
}

void APU::init() {
    // Setup buffer
	_blipBuffer.clock_rate(_clockRate);
	_blipBuffer.set_sample_rate(_sampleRate, 1000 / _frameRate);	
	
	// Setup synth
	_synthSquare1.volume(0.50);
	_synthSquare1.output(&_blipBuffer);

    _synthSquare2.volume(0.50);
	_synthSquare2.output(&_blipBuffer);

    _synthTriangle.volume(0.50);
	_synthTriangle.output(&_blipBuffer);

    _synthNoise.volume(0.50);
	_synthNoise.output(&_blipBuffer);

    _synthDmc.volume(0.50);
	_synthDmc.output(&_blipBuffer);
	
	// Start audio
	_audio.start(_sampleRate);
}

inline bool count(int& value, int reset) { 
    return --value < 0 ? (value=reset) , true : false; 
}

void APU::write(u8 index, u8 value) {
    APUChannel& ch = _channels[(index/4) % 5];
    switch(index<0x10 ? index%4 : index)
    {
        case 0: 
            if(ch.reg.LinearCounterDisable) {
                ch.linearCounter(value & 0x7F); 
            }
            ch.reg.reg0 = value; 
            break;
        case 1: 
            ch.reg.reg1 = value; 
            ch.sweepDelay(ch.reg.SweepRate); 
            break;
        case 2: ch.reg.reg2 = value; break;
        case 3:
            ch.reg.reg3 = value;
            if(_channelsEnabled[index/4]) {
                ch.lengthCounter(_lengthCounters[ch.reg.LengthCounterInit]);
            }
            ch.linearCounter(ch.reg.LinearCounterInit);
            ch.envDelay(ch.reg.EnvDecayRate);
            ch.envelope(15);
            if(index < 8) {
                ch.phase = 0;
            }
            break;
        case 0x10: 
            ch.reg.reg3 = value; 
            ch.reg.WaveLength = _DMCperiods[value&0x0F]; 
            break;
        case 0x12: 
            ch.reg.reg0 = value; 
            ch.address((ch.reg.reg0 | 0x300) << 6); 
            break;
        case 0x13: 
            ch.reg.reg1 = value; 
            ch.lengthCounter(ch.reg.PCMlength * 16 + 1); 
            break; // sample length
        case 0x11: ch.linearCounter(value & 0x7F); break; // dac value
        case 0x15: //channel enabler
            for(unsigned c=0; c<5; ++c) {
                _channelsEnabled[c] = value & (1 << c);
            }
            for(unsigned c=0; c<5; ++c) {
                if(!_channelsEnabled[c]) {
                    _channels[c].lengthCounter(0);
                } else if (c == 4 && _channels[c].lengthCounter()) {
                    _channels[c].lengthCounter(ch.reg.PCMlength * 16 + 1);
                }
            }
            break;
        case 0x17:
            _IRQdisable       = value & 0x40;
            _fiveCycleDivider = value & 0x80;
            _hz240counter     = { 0,0 };
            if(_IRQdisable) {
                _periodicIRQ = _DMC_IRQ = false;
            }
    }
}

u8 APU::read() {
    u8 result = 0;
    for(unsigned c=0; c<5; ++c) {
        result |= (_channels[c].lengthCounter() ? 1 << c : 0);
    }
    if(_periodicIRQ) {
        result |= 0x40; 
    } 
    _periodicIRQ = false;
    if(_DMC_IRQ) {
        result |= 0x80;
    }     
    _DMC_IRQ = false;
    _cpu->intr = false;
    return result;
}

void APU::tick() { // Invoked at CPU's rate. 
    // Divide CPU clock by 7457.5 to get a 240 Hz, which controls certain events.
    if((_hz240counter.lo += 2) >= 14915) {
        _hz240counter.lo -= 14915;
        if(++_hz240counter.hi >= 4+_fiveCycleDivider) {
            _hz240counter.hi = 0;
        }
        // 60 Hz interval: IRQ. IRQ is not invoked in five-cycle mode (48 Hz).
        if(!_IRQdisable && !_fiveCycleDivider && _hz240counter.hi==0){
            _cpu->intr = _periodicIRQ = true;
        }
        // Some events are invoked at 96 Hz or 120 Hz rate. Others, 192 Hz or 240 Hz.
        bool halfTick = (_hz240counter.hi&5)==1, fullTick = _hz240counter.hi < 4;
		unsigned c = 0;
        for(APUChannel& channel : _channels) {            
            int wl = channel.reg.WaveLength;
            // Length tick (all channels except DMC, but different disable bit for triangle wave)
			if (halfTick && channel.lengthCounter() && 
                !(c == 2 ? (bool)channel.reg.LinearCounterDisable : (bool)channel.reg.LengthCounterDisable)) {
				channel.lengthCounter(channel.lengthCounter() - 1);
			}
            // Sweep tick (square waves only)
            int sweepDelay = channel.sweepDelay();
            bool countResult = count(sweepDelay, channel.reg.SweepRate);
            channel.sweepDelay(sweepDelay);
            if (halfTick && c < 2 && countResult) {
                if(wl >= 8 && channel.reg.SweepEnable && channel.reg.SweepShift) {
                    int s = wl >> channel.reg.SweepShift, d[4] = {s, s, ~s, -s};
                    wl += d[channel.reg.SweepDecrease*2 + c];
					if (wl < 0x800) {
						channel.reg.WaveLength = wl;
					}
                }
            }
            // Linear tick (triangle wave only)
            if(fullTick && c == 2) {
				channel.linearCounter(channel.reg.LinearCounterDisable ?
                                      channel.reg.LinearCounterInit : 
                                      (channel.linearCounter() > 0 ? channel.linearCounter() - 1 : 0));
            }
            // Envelope tick (square and noise channels)
            int envDelay = channel.envDelay();
            countResult = count(envDelay, channel.reg.EnvDecayRate);
            channel.envDelay(envDelay);

            if(fullTick && c != 2 && countResult) {
                if(channel.envelope() > 0 || channel.reg.EnvDecayLoopEnable) {
					channel.envelope((channel.envelope()-1) & 15);
                }
            }			
			c++;
        }
    }

	if (_currentSample <= _cyclesPerFrame) {	
		_synthSquare1.update(_currentSample, channelTick(0));
		_synthSquare2.update(_currentSample, channelTick(1));
		_synthTriangle.update(_currentSample, channelTick(2));
		_synthNoise.update(_currentSample, channelTick(3));
		_synthDmc.update(_currentSample, channelTick(4));

        _currentSample++;        
    } 
	
    if (_currentSample == _cyclesPerFrame) {        
        _blipBuffer.end_frame(_cyclesPerFrame);
        long count = _blipBuffer.read_samples(_samples, BUFFER_SIZE);
        _audio.write(_samples, count);
        _currentSample = 0;
    }
}

int APU::channelTick(unsigned channelNumber) {
	return _channels[channelNumber].tick(channelNumber == 1 ? 0 : channelNumber, _channelsEnabled, _noisePeriods, _DMC_IRQ);
}