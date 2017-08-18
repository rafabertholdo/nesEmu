#include "Instructions/TAYInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TAYInstruction> registrar("TAYInstruction");
}

void TAYInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xA8;        
    insctructions[opcode] = Instruction(implict, opcode, "TAY", TAYInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t TAYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y = cpu.A;
    cpu.tick();
    return cpu.Y;
}