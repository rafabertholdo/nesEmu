#include "Instructions/BVCInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVCInstruction> registrar("BVCInstruction");    
}

void BVCInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x50;        
    insctructions[opcode] = Instruction(relative, opcode, "BVC", BVCInstruction::sharedAction);
}

uint_least16_t BVCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Overflow) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}