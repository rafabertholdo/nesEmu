#include "Instructions/INXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INXInstruction> registrar("INXInstruction");
    Instruction::Registrar2<INXInstruction> registrar2("INXInstruction");
}

void INXInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xE8;    
    insctructions.at(opcode) = make_unique<INXInstruction>(implict, opcode, "INX", AffectFlags::Negative | AffectFlags::Zero);
}

void INXInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xE8;    
    auto instruction = INXInstruction(implict, opcode, "INX", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(INXInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t INXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X++;
    cpu.tick();
    return cpu.X;
}

uint_least16_t INXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return INXInstruction::sharedAction(cpu, value);
}