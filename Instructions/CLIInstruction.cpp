#include "Instructions/CLIInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLIInstruction> registrar("CLIInstruction");
}

void CLIInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x58;    
    insctructions[opcode] = Instruction(implict, opcode, "CLI", ClearInstruction::sharedAction, AffectFlags::InterruptDisabled);
}