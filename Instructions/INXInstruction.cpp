#include "Instructions/INXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INXInstruction> registrar("INXInstruction");
}

void INXInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xE8;    
    insctructions[opcode] = Instruction(implict, opcode, "INX", INXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t INXInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.X++;
    cpu.tick();
    return cpu.X;
}
