#pragma once
#include "Utils.h"
#include "RegBit.h"
#include "APUChannel.h"
#include <array>
#include "Blip_Buffer.h"
#include "Sync_Audio.h"

class APU : public std::enable_shared_from_this<APU> {    
    std::shared_ptr<CPU> _cpu;
    static const u8 m_LengthCounters[32];
    static const u16 m_noisePeriods[16];
    static const u16 m_DMCperiods[16];
    static const long m_sampleRate;
    static const double m_clockRate;
    static const int m_frameRate;
    static const int m_cyclesPerFrame;
    static const int m_BufferSize = 10000;

    static Sync_Audio m_audio;
    static Blip_Buffer m_blipBuffer;
    static Blip_Synth<blip_good_quality,15> m_synthSquare1;
    static Blip_Synth<blip_good_quality,15> m_synthSquare2;
    static Blip_Synth<blip_good_quality,15> m_synthTriangle;
    static Blip_Synth<blip_med_quality,15> m_synthNoise;
    static Blip_Synth<blip_med_quality,127> m_synthDmc;

    blip_sample_t m_samples[m_BufferSize];

    bool m_fiveCycleDivider = false;
    bool m_IRQdisable = true;
    bool m_channelsEnabled[5] = { false };
    bool m_periodicIRQ = false;
    bool m_DMC_IRQ = false;

    APUChannel m_channels[5];
    int m_currentSample;	
	int channelTick(unsigned channelNumber);
    
    struct { 
        short lo;
        short hi; 
    }m_hz240counter = { 0,0 };
public:
    APU(const std::shared_ptr<CPU> &cpu);
    ~APU();

    void write(u8 index, u8 value);
    u8 read();
    void tick();
    void init();
};
   
