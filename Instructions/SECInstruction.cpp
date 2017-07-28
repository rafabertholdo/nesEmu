#include "Instructions/SECInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SECInstruction> registrar("SECInstruction");
    Instruction::Registrar2<SECInstruction> registrar2("SECInstruction");
}

void SECInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x38;    
    insctructions.at(opcode) = make_unique<SECInstruction>(implict, opcode, "SEC", AffectFlags::Carry);
}

void SECInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x38;    
    auto instruction = SECInstruction(implict, opcode, "SEC", AffectFlags::Carry);
    instruction.setLambda(SetInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}