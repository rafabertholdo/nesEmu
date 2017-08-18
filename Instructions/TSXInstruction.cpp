#include "Instructions/TSXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TSXInstruction> registrar("TSXInstruction");
}

void TSXInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xBA;    
    insctructions[opcode] = Instruction(implict, opcode, "TSX", TSXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t TSXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X = cpu.SP;
    cpu.tick();
    return cpu.X;
}