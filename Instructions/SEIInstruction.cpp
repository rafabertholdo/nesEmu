#include "Instructions/SEIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SEIInstruction> registrar("SEIInstruction");
    Instruction::Registrar2<SEIInstruction> registrar2("SEIInstruction");
}

void SEIInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x78;    
    insctructions.at(opcode) = make_unique<SEIInstruction>(implict, opcode, "SEI", AffectFlags::InterruptDisabled);
}

void SEIInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x78;    
    auto instruction = SEIInstruction(implict, opcode, "SEI", AffectFlags::InterruptDisabled);
    instruction.setLambda(SetInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}