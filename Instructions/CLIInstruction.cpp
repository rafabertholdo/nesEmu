#include "Instructions/CLIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLIInstruction> registrar("CLIInstruction");
    Instruction::Registrar2<CLIInstruction> registrar2("CLIInstruction");
}

void CLIInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x58;    
    insctructions.at(opcode) = make_unique<CLIInstruction>(implict, opcode, "CLI", AffectFlags::InterruptDisabled);
}

void CLIInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x58;    
    auto instruction = CLIInstruction(implict, opcode, "CLI", AffectFlags::InterruptDisabled);
    instruction.setLambda(ClearInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}