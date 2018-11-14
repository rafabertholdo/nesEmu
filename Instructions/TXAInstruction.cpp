#include "Instructions/TXAInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TXAInstruction> registrar("TXAInstruction");
}

void TXAInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x8A;    
    insctructions[opcode] = Instruction(implict, opcode, "TXA", TXAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t TXAInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.A = cpu.X;
    cpu.tick();
    return cpu.A;
}
