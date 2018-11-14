#include "Instructions/DEXInstruction.h"
#include "Instruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {    
    Instruction::Registrar<DEXInstruction> registrar("DEXInstruction");
}

void DEXInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xCA;        
    insctructions[opcode] = Instruction(implict, opcode, "DEX", DEXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
}

uint_least16_t DEXInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {    
    cpu.X--;
    cpu.tick();
    return cpu.X;
}
