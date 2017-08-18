#include "Instructions/CLCInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLCInstruction> registrar("CLCInstruction");
}

void CLCInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x18;        
    insctructions[opcode] = Instruction(implict, opcode, "CLC", ClearInstruction::sharedAction, AffectFlags::Carry);
}