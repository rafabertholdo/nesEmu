#include "Instructions/TXSInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TXSInstruction> registrar("TXSInstruction");
}

void TXSInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x9A;    
    insctructions[opcode] = Instruction(implict, opcode, "TXS", TXSInstruction::sharedAction);
}

uint_least16_t TXSInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.SP = cpu.X;
    cpu.tick();
    return cpu.SP;
}
