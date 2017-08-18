#include "Instructions/SEDInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SEDInstruction> registrar("SEDInstruction");
}

void SEDInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xF8;        
    insctructions[opcode] = Instruction(implict, opcode, "SED", SetInstruction::sharedAction, AffectFlags::DecimalMode);
}