#include "Instructions/CLCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLCInstruction> registrar("CLCInstruction");
    Instruction::Registrar2<CLCInstruction> registrar2("CLCInstruction");
}

void CLCInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x18;    
    insctructions.at(opcode) = make_unique<CLCInstruction>(implict, opcode, "CLC", AffectFlags::Carry);
}

void CLCInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x18;    
    auto instruction = CLCInstruction(implict, opcode, "CLC", AffectFlags::Carry);
    instruction.setLambda(ClearInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}