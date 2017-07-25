#ifndef CPU_H
#define CPU_H

#include <memory>
#include <vector>
#include <map>
#include <bitset>
#include "ROM.h"
#include "IO.h"
#include "Instruction.h"
#include "RegBit.h"
#include "Addressing.h"
#include <chrono>
#include <functional>
#include <fstream>

using namespace std;
class Instruction; //forward declaration

class PPU;
class APU;

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    int elapsed() const { 
        return std::chrono::duration_cast<mili_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<int, std::milli > mili_;
    std::chrono::time_point<clock_> beg_;
};

class CPU {
    vector<uint_least8_t> RAM;
    std::ifstream _testLogFile;

    shared_ptr<ROM> rom;
    shared_ptr<PPU> ppu;
    shared_ptr<IO> io;
    shared_ptr<APU> _apu;
    u32 tickCount;
    double executedInstructionsCount = 0;
    
    // Remaining clocks to end frame:
    const int totalCycles;
    int remainingCycles;
    int elapsed();

    bool testing;
    Timer timer;
    void test(const string &line, const vector<uint_least8_t> &instructionData, const string &menmonic);
    vector<Instruction> instructions;  
    //std::vector<std::reference_wrapper<Instruction>>;
    map<uint_least8_t, shared_ptr<Instruction>> instructionsMapping;  
    
    void executeInstruction(Instruction &instruction);
    uint_least8_t memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);    
    void identify(const vector<uint_least8_t> &instructionData, const Instruction &instruction);    
    void dmaOam(const uint_least8_t &value);
    bool handleInterruptions();
public:
    
    //registers
    uint_least8_t A=0, X=0, Y=0, SP=0;    
    bool running;       
    union /* Status flags: */
    {
        uint_least8_t raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptDisabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        RegBit<4> Break; //break        
        RegBit<6> Overflow; // overflow
        RegBit<7> Negative; // negative
    } Flags;

    uint_least16_t PC; //program counter
    bool reset=true, nmi=false, nmi_edge_detected=false, intr=false;

    CPU(const shared_ptr<IO> &io);
    CPU(const CPU &cpu);  // copy constructor
    ~CPU();
    void loadRom(const shared_ptr<ROM> &rom);
    uint_least8_t read(const uint_least16_t &address);
    u16 read(const uint_least16_t &address, const uint_least8_t &length);
    void write(const uint_least16_t &address, const uint_least8_t &value);

    void push(const uint_least8_t &value);
    uint_least8_t pop();

    void run();
    void dumpRegs();

    u16 getNmiVectorValue();
    u16 getResetVectorValue();
    u16 getBrkVectorValue();

    void setPPU(const shared_ptr<PPU> &ppu);
    void setAPU(const shared_ptr<APU> &apu);
    void tick();
};

#endif