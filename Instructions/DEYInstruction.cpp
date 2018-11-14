#include "Instructions/DEYInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<DEYInstruction> registrar("DEYInstruction");
}

void DEYInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x88;        
    insctructions[opcode] = Instruction(implict, opcode, "DEY", DEYInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t DEYInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.Y--;
    cpu.tick();
    return cpu.Y;
}
