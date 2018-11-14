#include "Instructions/CLDInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLDInstruction> registrar("CLDInstruction");
}

void CLDInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xD8;        
    insctructions[opcode] = Instruction(implict, opcode, "CLD", ClearInstruction::sharedAction, AffectFlags::DecimalMode);
}