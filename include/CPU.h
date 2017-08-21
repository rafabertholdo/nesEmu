#ifndef CPU_H
#define CPU_H


#include <memory>
#include <vector>
#include <map>
#include <bitset>

#include <fstream>

#include "ROM.h"
#include "IO.h"
#include "Instruction.h"
#include <array>
#include "RegBit.h"
#include "Addressing.h"
#include "Timer.h"

class PPU;
class APU;

static constexpr int kMaxPrgRomSize = 0x80000; //500kb

class CPU {
    std::vector<uint_least8_t> RAM;
    
    std::array<u32, PRG_PAGES> m_prgMap;
    std::array<u8, kMaxPrgRomSize> m_prg; 

    std::ifstream _testLogFile;
    
    std::shared_ptr<PPU> ppu;
    std::shared_ptr<IO> io;
    std::shared_ptr<APU> _apu;
    u32 tickCount;
    double executedInstructionsCount = 0;
    
    // Remaining clocks to end frame:
    const int totalCycles;
    int remainingCycles;
    int elapsed();

    bool testing;
    Timer timer;
    void test(const std::string &line, const std::vector<uint_least8_t> &instructionData, const std::string &menmonic);
    InstructionArray instructions;          
    
    void executeInstruction(Instruction &instruction);
    uint_least8_t memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);    
    void identify(const std::vector<uint_least8_t> &instructionData, const Instruction &instruction);    
    void dmaOam(const uint_least8_t &value);
    bool handleInterruptions();    
    uint_least8_t prgAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);
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

    CPU(const std::shared_ptr<IO> &io);
    CPU(const CPU &cpu);  // copy constructor
    ~CPU();
    void loadRom(const std::shared_ptr<ROM> &rom);
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

    void setPPU(const std::shared_ptr<PPU> &ppu);
    void setAPU(const std::shared_ptr<APU> &apu);
    void tick();
};

#endif