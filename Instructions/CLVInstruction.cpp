#include "Instructions/CLVInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLVInstruction> registrar("CLVInstruction");
}

void CLVInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0xB8;    
    insctructions[opcode] = Instruction(implict, opcode, "CLV", ClearInstruction::sharedAction, AffectFlags::Overflow);
}