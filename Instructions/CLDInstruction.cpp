#include "Instructions/CLDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLDInstruction> registrar("CLDInstruction");
    Instruction::Registrar2<CLDInstruction> registrar2("CLDInstruction");
}

void CLDInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xD8;    
    insctructions.at(opcode) = make_unique<CLDInstruction>(implict, opcode, "CLD",  AffectFlags::DecimalMode);
}

void CLDInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xD8;    
    auto instruction = CLDInstruction(implict, opcode, "CLD", AffectFlags::DecimalMode);
    instruction.setLambda(ClearInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}