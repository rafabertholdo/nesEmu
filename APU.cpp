#include "APU.h"
#include <iostream>

const u8 APU::LengthCounters[32] = { 10,254,20, 2,40, 4,80, 6,160, 8,60,10,14,12,26,14,
                               12, 16,24,18,48,20,96,22,192,24,72,26,16,28,32,30 };

const u16 APU::NoisePeriods[16] = { 2,4,8,16,32,48,64,80,101,127,190,254,381,508,1017,2034 };
const u16 APU::DMCperiods[16] = { 428,380,340,320,286,254,226,214,190,160,142,128,106,84,72,54 };                               

APU::APU(const std::shared_ptr<CPU> &cpu) {
    _cpu = cpu;    

    bufs = NULL;
	free_sem = NULL;
	write_buf = 0;
	write_pos = 0;
	read_buf = 0;
	sound_open = false;
}

APU::~APU() {
    if (sound_open) {
		SDL_PauseAudio( true );
		SDL_CloseAudio();
	}
	
	if (free_sem)
		SDL_DestroySemaphore(free_sem);
	
	delete [] bufs;
}


void APU::fill_buffer(u8 *audioData, int length)
{
	if (SDL_SemValue(free_sem) < _buf_count - 1) {
		memcpy(audioData, buf(read_buf), length);
		read_buf = (read_buf + 1) % _buf_count;
		SDL_SemPost(free_sem);
	} else {
		memset(audioData, 0, length);
	}
}

void APU::fill_buffer_(void* userData, u8* audioData, int length) {
	((APU*) userData)->fill_buffer(audioData, length);
}

inline u8* APU::buf(int index) {
	return bufs + (long)index * _buf_size;
}


void APU::init() {
    for(int i=0;i<5;i++){
        channels.push_back(APUChannel(shared_from_this(), _cpu));
    }

    bufs = new u8 [(long) _buf_size * _buf_count];
	if ( !bufs )
		cout << "Out of memory";
	
	free_sem = SDL_CreateSemaphore( _buf_count - 1 );
	if ( !free_sem )
		cout << "Couldn't create semaphore";

    SDL_AudioSpec as;
	as.freq = 96000;
	as.format = AUDIO_S16SYS;
	as.channels = 1;
	as.silence = 0;
	as.samples = _buf_size;
	as.size = 0;
	as.callback = fill_buffer_;
	as.userdata = this;
	if ( SDL_OpenAudio( &as, NULL ) < 0 ) {
        std::cout <<  "Couldn't open SDL audio"  << std::endl;
    }
		
	SDL_PauseAudio( false );
	sound_open = true;
}


bool APU::count(int& v, int reset) { 
    return --v < 0 ? (v=reset),true : false; 
}

void APU::Write(u8 index, u8 value) {
    APUChannel& ch = channels[(index/4) % 5];
    switch(index<0x10 ? index%4 : index)
    {
        case 0: if(ch.reg.LinearCounterDisable) ch.linear_counter=value&0x7F; ch.reg.reg0 = value; break;
        case 1: ch.reg.reg1 = value; ch.sweep_delay = ch.reg.SweepRate; break;
        case 2: ch.reg.reg2 = value; break;
        case 3:
            ch.reg.reg3 = value;
            if(ChannelsEnabled[index/4])
                ch.length_counter = LengthCounters[ch.reg.LengthCounterInit];
            ch.linear_counter = ch.reg.LinearCounterInit;
            ch.env_delay      = ch.reg.EnvDecayRate;
            ch.envelope       = 15;
            if(index < 8) ch.phase = 0;
            break;
        case 0x10: ch.reg.reg3 = value; ch.reg.WaveLength = DMCperiods[value&0x0F]; break;
        case 0x12: ch.reg.reg0 = value; ch.address = (ch.reg.reg0 | 0x300) << 6; break;
        case 0x13: ch.reg.reg1 = value; ch.length_counter = ch.reg.PCMlength*16 + 1; break; // sample length
        case 0x11: ch.linear_counter = value & 0x7F; break; // dac value
        case 0x15:
            for(unsigned c=0; c<5; ++c)
                ChannelsEnabled[c] = value & (1 << c);
            for(unsigned c=0; c<5; ++c)
                if(!ChannelsEnabled[c])
                    channels[c].length_counter = 0;
                else if(c == 4 && channels[c].length_counter == 0)
                    channels[c].length_counter = ch.reg.PCMlength*16 + 1;
            break;
        case 0x17:
            IRQdisable       = value & 0x40;
            FiveCycleDivider = value & 0x80;
            hz240counter     = { 0,0 };
            if(IRQdisable) PeriodicIRQ = DMC_IRQ = false;
    }
}

u8 APU::Read()
{
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

void APU::tick() // Invoked at CPU's rate.
{
    // Divide CPU clock by 7457.5 to get a 240 Hz, which controls certain events.
    if((hz240counter.lo += 2) >= 14915)
    {
        hz240counter.lo -= 14915;
        if(++hz240counter.hi >= 4+FiveCycleDivider) hz240counter.hi = 0;
        // 60 Hz interval: IRQ. IRQ is not invoked in five-cycle mode (48 Hz).
        if(!IRQdisable && !FiveCycleDivider && hz240counter.hi==0)
            _cpu->intr = PeriodicIRQ = true;
        // Some events are invoked at 96 Hz or 120 Hz rate. Others, 192 Hz or 240 Hz.
        bool HalfTick = (hz240counter.hi&5)==1, FullTick = hz240counter.hi < 4;
        for(unsigned c=0; c<4; ++c)
        {
            APUChannel& ch = channels[c];
            int wl = ch.reg.WaveLength;
            // Length tick (all channels except DMC, but different disable bit for triangle wave)
            if(HalfTick && ch.length_counter && !(c==2 ? ch.reg.LinearCounterDisable : ch.reg.LengthCounterDisable))
                ch.length_counter -= 1;
            // Sweep tick (square waves only)
            if(HalfTick && c < 2 && count(ch.sweep_delay, ch.reg.SweepRate))
                if(wl >= 8 && ch.reg.SweepEnable && ch.reg.SweepShift)
                {
                    int s = wl >> ch.reg.SweepShift, d[4] = {s, s, ~s, -s};
                    wl += d[ch.reg.SweepDecrease*2 + c];
                    if(wl < 0x800) ch.reg.WaveLength = wl;
                }
            // Linear tick (triangle wave only)
            if(FullTick && c == 2)
                ch.linear_counter = ch.reg.LinearCounterDisable
                ? ch.reg.LinearCounterInit
                : (ch.linear_counter > 0 ? ch.linear_counter - 1 : 0);
            // Envelope tick (square and noise channels)
            if(FullTick && c != 2 && count(ch.env_delay, ch.reg.EnvDecayRate))
                if(ch.envelope > 0 || ch.reg.EnvDecayLoopEnable)
                    ch.envelope = (ch.envelope-1) & 15;
        }
    }
    // Mix the audio: Get the momentary sample from each channel and mix them.
    
    #define s(c) channels[c].tick(c==1 ? 0 : c)
    auto v = [](float m,float n, float d) { return n!=0.f ? m/n : d; };
    short sample = 30000 *
      (v(95.88f,  (100.f + v(8128.f, s(0) + s(1), -100.f)), 0.f)
    +  v(159.79f, (100.f + v(1.0, s(2)/8227.f + s(3)/12241.f + s(4)/22638.f, -100.f)), 0.f)
      - 0.5f
      );
    #undef s
    // I cheat here: I did not bother to learn how to use SDL mixer, let alone use it in <5 lines of code,
    // so I simply use a combination of external programs for outputting the audio.
    // Hooray for Unix principles! A/V sync will be ensured in post-process.
    //return; // Disable sound because already device is in use
    //static FILE* fp = popen("resample mr1789800 r48000 | aplay -fdat 2>/dev/null", "w");
    //fputc(sample, fp);
    //fputc(sample/256, fp);

/*

    // NOTE: Sound test
    int SamplesPerSecond = 48000;
    int ToneHz = 256;
    int_least16_t ToneVolume = 3000;
    u32 RunningSampleIndex = 0;
    int SquareWavePeriod = SamplesPerSecond / ToneHz;
    int HalfSquareWavePeriod = SquareWavePeriod / 2;
    int BytesPerSample = sizeof(int_least16_t) * 2;

    int BytesToWrite = 800 * BytesPerSample;

    void *SoundBuffer = malloc(BytesToWrite);
    int_least16_t *SampleOut = (int_least16_t *)SoundBuffer;
    int SampleCount = BytesToWrite/BytesPerSample;

    for(int SampleIndex = 0;
    SampleIndex < SampleCount;
    ++SampleIndex)
    {
        int_least16_t SampleValue = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
        *SampleOut++ = sample;
        *SampleOut++ = sample/256;
    }
    SDL_QueueAudio(1, SoundBuffer, BytesToWrite);
    free(SoundBuffer);
    int SoundIsPlaying = 0;
    if (!SoundIsPlaying)
    {
        SDL_PauseAudio(0);
        SoundIsPlaying = true;
    }   
    */
}