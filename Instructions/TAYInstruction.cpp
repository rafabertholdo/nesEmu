#include "Instructions/TAYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TAYInstruction> registrar("TAYInstruction");
    Instruction::Registrar2<TAYInstruction> registrar2("TAYInstruction");
}

void TAYInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xA8;    
    insctructions.at(opcode) = make_unique<TAYInstruction>(implict, opcode, "TAY", AffectFlags::Negative | AffectFlags::Zero);
}

void TAYInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xA8;    
    auto instruction = TAYInstruction(implict, opcode, "TAY", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(TAYInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TAYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y = cpu.A;
    cpu.tick();
    return cpu.Y;
}

uint_least16_t TAYInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TAYInstruction::sharedAction(cpu, value);
}