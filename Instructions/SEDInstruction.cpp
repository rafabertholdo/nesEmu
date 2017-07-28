#include "Instructions/SEDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SEDInstruction> registrar("SEDInstruction");
    Instruction::Registrar2<SEDInstruction> registrar2("SEDInstruction");
}

void SEDInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xF8;    
    insctructions.at(opcode) = make_unique<SEDInstruction>(implict, opcode, "SED", AffectFlags::DecimalMode);
}

void SEDInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xF8;    
    auto instruction = SEDInstruction(implict, opcode, "SED", AffectFlags::DecimalMode);
    instruction.setLambda(SetInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}