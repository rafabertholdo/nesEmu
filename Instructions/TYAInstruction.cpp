#include "Instructions/TYAInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TYAInstruction> registrar("TYAInstruction");
}

void TYAInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x98;    
    insctructions[opcode] = Instruction(implict, opcode, "TYA", TYAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t TYAInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.A = cpu.Y;
    cpu.tick();
    return cpu.A;
}
