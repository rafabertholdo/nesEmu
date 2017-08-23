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
    std::shared_ptr<PPU> m_ppu;
    std::shared_ptr<IO> m_io;
    std::shared_ptr<APU> m_apu;

    std::vector<u8> m_RAM;    
    std::array<u32, PRG_PAGES> m_prgMap;
    std::array<u8, kMaxPrgRomSize> m_prg; 

    InstructionArray m_instructions;    
    
    bool m_reset;    
        
    double m_executedInstructionsCount;
    const int m_totalCycles;
    // Remaining clocks to end frame:
    int m_remainingCycles;    

    bool m_testing;
    Timer m_timer;    
    std::ifstream m_testLogFile;     
    
    void executeInstruction(Instruction &instruction);
    u8 memAccess(const u16 &address, const u8 &value, const bool &write);    
    void identify(const std::vector<u8> &instructionData, const Instruction &instruction);    
    void dmaOam(const u8 &value);    
    u8 prgAccess(const u16 &address, const u8 &value, const bool &write);
    int elapsed();
    void test(const std::string &line, const std::vector<u8> &instructionData, const std::string &menmonic);    
    void dumpRegs();
public:

    //registers
    u8 A;
    u8 X;
    u8 Y;
    u8 SP;    
           
    union /* Status flags: */
    {
        u8 raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptDisabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        RegBit<4> Break; //break        
        RegBit<6> Overflow; // overflow
        RegBit<7> Negative; // negative
    } Flags;

    u16 PC; //program counter

    bool nmi;
    bool nmiEdgeDetected;
    bool intr;

    CPU(const std::shared_ptr<IO> &io);
    CPU(const CPU &cpu);  // copy constructor
    ~CPU();
    
    u8 read(const u16 &address);
    u16 read(const u16 &address, const u8 &length);
    void write(const u16 &address, const u8 &value);

    void push(const u8 &value);
    u8 pop();

    void run();
    
    u16 getNmiVectorValue();
    u16 getResetVectorValue();
    u16 getBrkVectorValue();

    void loadRom(const std::shared_ptr<ROM> &rom);
    void setPPU(const std::shared_ptr<PPU> &ppu);
    void setAPU(const std::shared_ptr<APU> &apu);
    void tick();
};

#endif