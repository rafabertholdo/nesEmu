#include "Instructions/DEYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DEYInstruction> registrar("DEYInstruction");
    Instruction::Registrar2<DEYInstruction> registrar2("DEYInstruction");
}

void DEYInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x88;    
    insctructions.at(opcode) = make_unique<DEYInstruction>(implict, opcode, "DEY", AffectFlags::Negative | AffectFlags::Zero);
}

void DEYInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x88;    
    auto instruction = DEYInstruction(implict, opcode, "DEY", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(DEYInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t DEYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y--;
    cpu.tick();
    return cpu.Y;
}

uint_least16_t DEYInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return DEYInstruction::sharedAction(cpu, value);
}