#include "Instructions/BEQInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BEQInstruction> registrar("BEQInstruction");    
}


void BEQInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xF0;
    insctructions[opcode] = Instruction(relative, opcode, "BEQ", BEQInstruction::sharedAction);
}

uint_least16_t BEQInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Zero) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}