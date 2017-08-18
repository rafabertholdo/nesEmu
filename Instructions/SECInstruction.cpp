#include "Instructions/SECInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SECInstruction> registrar("SECInstruction");
}

void SECInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x38;        
    insctructions[opcode] = Instruction(implict, opcode, "SEC", SetInstruction::sharedAction, AffectFlags::Carry);
}