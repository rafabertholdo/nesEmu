#include "Instructions/BRKInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BRKInstruction> registrar("BRKInstruction");
}

vector<shared_ptr<Instruction>> BRKInstruction::createInstructions() {
    auto instruction = make_shared<BRKInstruction>(implict,0x00,1,"BRK");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t BRKInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    uint_least8_t flagsToPush = cpu.Flags.raw;
    flagsToPush |= 0b110000; //bit 4 and 5 is always set on push
    cpu.push(flagsToPush);
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);
    cpu.Flags.raw |= 0b10000; //bit 4 is set on break
    
    //IRQ interrupt vector at $FFFE/F is loaded into the PC     
    u16 IRQAddress = 0xFFFE;
    u8 lsb = cpu.read(IRQAddress);
    u8 msb = cpu.read(IRQAddress + 1);
    cpu.PC = Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}) + cpu.Y;
    return cpu.PC;
}