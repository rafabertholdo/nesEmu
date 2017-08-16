#include "APU.h"
#include <iostream>
#include "Blip_Buffer.h"
#include "Sync_Audio.h"

const u8 APU::LengthCounters[32] = { 10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
                               12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30 };

const u16 APU::NoisePeriods[16] = { 2,4,8,16,32,48,64,80,101,127,190,254,381,508,1017,2034 };
const u16 APU::DMCperiods[16] = { 428,380,340,320,286,254,226,214,190,160,142,128,106,84,72,54 };                               

const long sample_rate = 96000;    // 44.1 kHz sample rate
const double clock_rate = 1789773; // 1 MHz clock rate
const int frame_rate = 60;         // 60 frames of sound per second
const int cyclesPerFrame = 29781;

static Sync_Audio audio;
static Blip_Buffer blipBuffer;
static Blip_Synth<blip_good_quality,15> synthSquare1;
static Blip_Synth<blip_good_quality,15> synthSquare2;
static Blip_Synth<blip_good_quality,15> synthTriangle;
static Blip_Synth<blip_med_quality,15> synthNoise;
static Blip_Synth<blip_med_quality,127> synthDmc;

// temporary buffer to read samples into
const int buf_size = 10000;
static blip_sample_t samples[buf_size];

APU::APU(const std::shared_ptr<CPU> &cpu) {
    _cpu = cpu;    

    currentSample = 0;
}

APU::~APU() {
    audio.stop();	
}

void APU::init() {
    for(int i=0;i<5;i++){
        channels.push_back(APUChannel(shared_from_this(), _cpu));
    }
    // Setup buffer
	blipBuffer.clock_rate(clock_rate);
	blipBuffer.set_sample_rate(sample_rate, 1000 / frame_rate);	
	
	// Setup synth
	synthSquare1.volume(1.00);
	synthSquare1.output(&blipBuffer);

    synthSquare2.volume(1.00);
	synthSquare2.output(&blipBuffer);

    synthTriangle.volume(1.00);
	synthTriangle.output(&blipBuffer);

    synthNoise.volume(1.00);
	synthNoise.output(&blipBuffer);

    synthDmc.volume(1.00);
	synthDmc.output(&blipBuffer);
	
	// Start audio
	audio.start(sample_rate);
}

bool APU::count(int& value, int reset) { 
    return --value < 0 ? (value=reset) , true : false; 
}

void APU::Write(u8 index, u8 value) {
    APUChannel& ch = channels[(index/4) % 5];
    switch(index<0x10 ? index%4 : index)
    {
        case 0: 
            if(ch.reg.LinearCounterDisable) {
                ch.linear_counter = value & 0x7F; 
            }
            ch.reg.reg0 = value; 
            break;
        case 1: 
            ch.reg.reg1 = value; 
            ch.sweep_delay = ch.reg.SweepRate; 
            break;
        case 2: ch.reg.reg2 = value; break;
        case 3:
            ch.reg.reg3 = value;
            if(ChannelsEnabled[index/4]) {
                ch.length_counter = LengthCounters[ch.reg.LengthCounterInit];
            }
            ch.linear_counter = ch.reg.LinearCounterInit;
            ch.env_delay      = ch.reg.EnvDecayRate;
            ch.envelope       = 15;
            if(index < 8) {
                ch.phase = 0;
            }
            break;
        case 0x10: 
            ch.reg.reg3 = value; 
            ch.reg.WaveLength = DMCperiods[value&0x0F]; 
            break;
        case 0x12: 
            ch.reg.reg0 = value; 
            ch.address = (ch.reg.reg0 | 0x300) << 6; 
            break;
        case 0x13: 
            ch.reg.reg1 = value; 
            ch.length_counter = ch.reg.PCMlength * 16 + 1; 
            break; // sample length
        case 0x11: ch.linear_counter = value & 0x7F; break; // dac value
        case 0x15: //channel enabler
            for(unsigned c=0; c<5; ++c) {
                ChannelsEnabled[c] = value & (1 << c);
            }
            for(unsigned c=0; c<5; ++c) {
                if(!ChannelsEnabled[c]) {
                    channels[c].length_counter = 0;
                } else if (c == 4 && channels[c].length_counter == 0) {
                    channels[c].length_counter = ch.reg.PCMlength * 16 + 1;
                }
            }
            break;
        case 0x17:
            IRQdisable       = value & 0x40;
            FiveCycleDivider = value & 0x80;
            hz240counter     = { 0,0 };
            if(IRQdisable) {
                PeriodicIRQ = DMC_IRQ = false;
            }
    }
}

u8 APU::Read() {
    u8 result = 0;
    for(unsigned c=0; c<5; ++c) {
        result |= (channels[c].length_counter ? 1 << c : 0);
    }
    if(PeriodicIRQ) {
        result |= 0x40; 
    } 
    PeriodicIRQ = false;
    if(DMC_IRQ) {
        result |= 0x80;
    }     
    DMC_IRQ = false;
    _cpu->intr = false;
    return result;
}

void APU::tick() { // Invoked at CPU's rate. 
    // Divide CPU clock by 7457.5 to get a 240 Hz, which controls certain events.
    if((hz240counter.lo += 2) >= 14915) {
        hz240counter.lo -= 14915;
        if(++hz240counter.hi >= 4+FiveCycleDivider) {
            hz240counter.hi = 0;
        }
        // 60 Hz interval: IRQ. IRQ is not invoked in five-cycle mode (48 Hz).
        if(!IRQdisable && !FiveCycleDivider && hz240counter.hi==0){
            _cpu->intr = PeriodicIRQ = true;
        }
        // Some events are invoked at 96 Hz or 120 Hz rate. Others, 192 Hz or 240 Hz.
        bool HalfTick = (hz240counter.hi&5)==1, FullTick = hz240counter.hi < 4;
        for(unsigned c=0; c<4; ++c) {
            APUChannel& ch = channels[c];
            int wl = ch.reg.WaveLength;
            // Length tick (all channels except DMC, but different disable bit for triangle wave)
            if(HalfTick && ch.length_counter && !(c==2 ? ch.reg.LinearCounterDisable : ch.reg.LengthCounterDisable))
                ch.length_counter -= 1;
            // Sweep tick (square waves only)
            if(HalfTick && c < 2 && count(ch.sweep_delay, ch.reg.SweepRate)) {
                if(wl >= 8 && ch.reg.SweepEnable && ch.reg.SweepShift)
                {
                    int s = wl >> ch.reg.SweepShift, d[4] = {s, s, ~s, -s};
                    wl += d[ch.reg.SweepDecrease*2 + c];
                    if(wl < 0x800) ch.reg.WaveLength = wl;
                }
            }
            // Linear tick (triangle wave only)
            if(FullTick && c == 2) {
                ch.linear_counter = ch.reg.LinearCounterDisable
                ? ch.reg.LinearCounterInit
                : (ch.linear_counter > 0 ? ch.linear_counter - 1 : 0);
            }
            // Envelope tick (square and noise channels)
            if(FullTick && c != 2 && count(ch.env_delay, ch.reg.EnvDecayRate)) {
                if(ch.envelope > 0 || ch.reg.EnvDecayLoopEnable) {
                    ch.envelope = (ch.envelope-1) & 15;
                }
            }
        }
    }
    // Mix the audio: Get the momentary sample from each channel and mix them.
    
    #define channelTick(channelNumber) channels[channelNumber].tick(channelNumber == 1 ? 0 : channelNumber)
    /*
    auto v = [](float m, float n, float d) { 
        return n != 0.f ? m/n : d; 
    };
    short sample = 30000 *
      (v(95.88f,  (100.f + v(8128.f, channelTick(0) + channelTick(1), -100.f)), 0.f)
    +  v(159.79f, (100.f + v(1.0, channelTick(2)/8227.f + channelTick(3)/12241.f + channelTick(4)/22638.f, -100.f)), 0.f)
      - 0.5f
      );
    */    

	if (currentSample <= cyclesPerFrame) {                
        synthSquare1.update(currentSample, channelTick(0));
        synthSquare2.update(currentSample, channelTick(1));
        synthTriangle.update(currentSample, channelTick(2));
        synthNoise.update(currentSample, channelTick(3));
        synthDmc.update(currentSample, channelTick(4));        

        currentSample++;        
    } 

    if (currentSample == cyclesPerFrame) {        
        blipBuffer.end_frame(cyclesPerFrame);
        long count = blipBuffer.read_samples(samples, buf_size);
        audio.write(samples, count);
        currentSample = 0;
    }
    #undef s   
}