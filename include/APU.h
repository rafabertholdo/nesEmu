#pragma once
#include "Utils.h"
#include "RegBit.h"
#include "APUChannel.h"
#include <array>
#include "Blip_Buffer.h"
#include "Sync_Audio.h"

static const int BUFFER_SIZE = 10000;

class APU : public std::enable_shared_from_this<APU> {    
    std::shared_ptr<CPU> _cpu;
    static const u8 _lengthCounters[32];
    static const u16 _noisePeriods[16];
    static const u16 _DMCperiods[16];
    static const long _sampleRate;
    static const double _clockRate;
    static const int _frameRate;
    static const int _cyclesPerFrame;

    static Sync_Audio _audio;
    static Blip_Buffer _blipBuffer;
    static Blip_Synth<blip_good_quality,15> _synthSquare1;
    static Blip_Synth<blip_good_quality,15> _synthSquare2;
    static Blip_Synth<blip_good_quality,15> _synthTriangle;
    static Blip_Synth<blip_med_quality,15> _synthNoise;
    static Blip_Synth<blip_med_quality,127> _synthDmc;

    blip_sample_t _samples[BUFFER_SIZE];

    bool _fiveCycleDivider = false;
    bool _IRQdisable = true;
    bool _channelsEnabled[5] = { false };
    bool _periodicIRQ = false;
    bool _DMC_IRQ = false;

    APUChannel _channels[5];
    int _currentSample;	
	int channelTick(unsigned channelNumber);
    
    struct { 
        short lo;
        short hi; 
    }_hz240counter = { 0,0 };
public:
    APU(const std::shared_ptr<CPU> &cpu);
    ~APU();

    void write(u8 index, u8 value);
    u8 read();
    void tick();
    void init();
};
   
