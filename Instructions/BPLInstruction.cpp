#include "Instructions/BPLInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BPLInstruction> registrar("BPLInstruction");    
}

void BPLInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x10;    
    insctructions[opcode] = Instruction(relative, opcode, "BPL", BPLInstruction::sharedAction);
}

uint_least16_t BPLInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Negative) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}