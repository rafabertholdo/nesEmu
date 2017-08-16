#pragma once
#include "Utils.h"
#include "RegBit.h"
#include "CPU.h"

class APU;

class APUChannel {
    std::shared_ptr<APU> _apu;
    std::shared_ptr<CPU> _cpu;
public:
    APUChannel(const shared_ptr<APU> &apu, const shared_ptr<CPU> &cpu);
    ~APUChannel();

    int length_counter, linear_counter, address, envelope;
    int sweep_delay, env_delay, wave_counter, hold, phase, level;
    union // Per-channel register file
    {
        // 4000, 4004, 400C, 4012:            // 4001, 4005, 4013:            // 4002, 4006, 400A, 400E:
        RegBit<0,8,u32> reg0;                 RegBit< 8,8,u32> reg1;          RegBit<16,8,u32> reg2;
        RegBit<6,2,u32> DutyCycle;            RegBit< 8,3,u32> SweepShift;    RegBit<16,4,u32> NoiseFreq;
        RegBit<4,1,u32> EnvDecayDisable;      RegBit<11,1,u32> SweepDecrease; RegBit<23,1,u32> NoiseType;
        RegBit<0,4,u32> EnvDecayRate;         RegBit<12,3,u32> SweepRate;     RegBit<16,11,u32> WaveLength;
        RegBit<5,1,u32> EnvDecayLoopEnable;   RegBit<15,1,u32> SweepEnable;   // 4003, 4007, 400B, 400F, 4010:
        RegBit<0,4,u32> FixedVolume;          RegBit< 8,8,u32> PCMlength;     RegBit<24,8,u32> reg3;
        RegBit<5,1,u32> LengthCounterDisable;                                 RegBit<27,5,u32> LengthCounterInit;
        RegBit<0,7,u32> LinearCounterInit;                                    RegBit<30,1,u32> LoopEnabled;
        RegBit<7,1,u32> LinearCounterDisable;                                 RegBit<31,1,u32> IRQenable;
    } reg;

    // Function for updating the wave generators and taking the sample for each channel.    
    int tick(unsigned c, bool channelsEnabled[], const u16 noisePeriods[], bool &dmcIrq);
};
   