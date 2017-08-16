#pragma once
#include "Utils.h"
#include "RegBit.h"
#include "APUChannel.h"
#include <array>

class APU : public std::enable_shared_from_this<APU> {    
    std::shared_ptr<CPU> _cpu;
    
    int currentSample;	
    int amplitude = 100;	
	int APU::channelTick(unsigned channelNumber);
public:
    APU(const std::shared_ptr<CPU> &cpu);
    ~APU();
    
    static const u8 LengthCounters[32];
    static const u16 NoisePeriods[16];
    static const u16 DMCperiods[16];

    bool FiveCycleDivider = false, IRQdisable = true, ChannelsEnabled[5] = { false };
    bool PeriodicIRQ = false, DMC_IRQ = false;
    static bool count(int& v, int reset);

    APUChannel channels[5];

    struct { short lo, hi; } hz240counter = { 0,0 };

    void Write(u8 index, u8 value);

    u8 Read();

    void tick();
    void init();
};
   
