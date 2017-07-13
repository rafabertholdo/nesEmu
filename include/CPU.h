#ifndef CPU_H
#define CPU_H

#include <memory>
#include <vector>
#include <map>
#include <bitset>
#include "Rom.h"
#include "Instruction.h"
#include "RegBit.h"
#include "PPU.h"
#include "Addressing.h"

using namespace std;
class Instruction; //forward declaration

class CPU {
    vector<uint_least8_t> RAM;
    const Rom *rom;
    PPU ppu;
    bool reset;
    bool running;       

    vector<shared_ptr<Instruction>> instructionVector;    
    map<uint_least8_t, shared_ptr<Instruction>> instructionsMapping;  
    uint_least8_t memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);
    std::map<AddressingMode, shared_ptr<Addressing>> addressingModes;
public:
    
    //registers
    uint_least8_t A=0, X=0, Y=0, SP=0;    

    union /* Status flags: */
    {
        uint_least8_t raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptEnabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        // 4,5 (0x10,0x20) don't exist
        RegBit<6> Overvlow; // overflow
        RegBit<7> Negative; // negative
    } Flags;

    uint_least16_t PC; //program counter

    CPU();
    ~CPU();
    void loadRom(const Rom &rom);
    uint_least8_t read(const uint_least16_t &address);
    vector<uint_least8_t> read(const uint_least16_t &address, const uint_least8_t &length);
    void write(const uint_least16_t &address, const uint_least8_t &value);
    void run();
    void dumpRegs();
};

#endif