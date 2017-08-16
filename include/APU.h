#pragma once
#include "Utils.h"
#include "RegBit.h"
#include "APUChannel.h"

class APU : public std::enable_shared_from_this<APU> {    
    std::shared_ptr<CPU> _cpu;
    bool _sound_open;
    
    int _buf_size = 2048;
	int _buf_count = 3;
	SDL_sem* volatile free_sem;
	int volatile read_buf;
	int write_buf;
	int write_pos;
	bool sound_open;
    u8* volatile bufs;
    u8* buf(int index);
    void fill_buffer(u8 *audioData, int length);
    static void fill_buffer_(void *userData, u8 *audioData, int length);
    void writeBuffer(const short* sample, int count);
public:
    APU(const std::shared_ptr<CPU> &cpu);
    ~APU();
    
    static const u8 LengthCounters[32];
    static const u16 NoisePeriods[16];
    static const u16 DMCperiods[16];

    bool FiveCycleDivider = false, IRQdisable = true, ChannelsEnabled[5] = { false };
    bool PeriodicIRQ = false, DMC_IRQ = false;
    static bool count(int& v, int reset);

    std::vector<APUChannel> channels;

    struct { short lo, hi; } hz240counter = { 0,0 };

    void Write(u8 index, u8 value);

    u8 Read();

    void tick();
    void init();
};
   
