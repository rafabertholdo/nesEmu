#include "Instructions/INYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INYInstruction> registrar("INYInstruction");
    Instruction::Registrar2<INYInstruction> registrar2("INYInstruction");
}

void INYInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xC8;    
    insctructions.at(opcode) = make_unique<INYInstruction>(implict, opcode, "INY", AffectFlags::Negative | AffectFlags::Zero);
}

void INYInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xC8;    
    auto instruction = INYInstruction(implict, opcode, "INY", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(INYInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t INYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y++;
    cpu.tick();
    return cpu.Y;
}

uint_least16_t INYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return INYInstruction::sharedAction(cpu, value);
}