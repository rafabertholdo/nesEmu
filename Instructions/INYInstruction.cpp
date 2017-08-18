#include "Instructions/INYInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INYInstruction> registrar("INYInstruction");
}

void INYInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xC8;        
    insctructions[opcode] = INYInstruction(implict, opcode, "INY", INYInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t INYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y++;
    cpu.tick();
    return cpu.Y;
}