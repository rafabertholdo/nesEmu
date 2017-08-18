#include "Instructions/TAXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<TAXInstruction> registrar("TAXInstruction");
}

void TAXInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xAA;        
    insctructions[opcode] = Instruction(implict, opcode, "TAX", TAXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t TAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X = cpu.A;
    cpu.tick();
    return cpu.X;
}