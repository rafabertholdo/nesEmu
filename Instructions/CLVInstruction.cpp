#include "Instructions/CLVInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLVInstruction> registrar("CLVInstruction");
    Instruction::Registrar2<CLVInstruction> registrar2("CLVInstruction");
}

void CLVInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xB8;    
    insctructions.at(opcode) = make_unique<CLVInstruction>(implict, opcode, "CLV", AffectFlags::Overflow);
}

void CLVInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xB8;    
    auto instruction = CLVInstruction(implict, opcode, "CLV", AffectFlags::Overflow);
    instruction.setLambda(ClearInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}