#include "Instructions/PLAInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<PLAInstruction> registrar("PLAInstruction");
}

void PLAInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x68;    
    insctructions[opcode] = Instruction(implict, opcode, "PLA", PLAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t PLAInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {
    cpu.tick();
    cpu.tick();
    return cpu.A = cpu.pop();
}
