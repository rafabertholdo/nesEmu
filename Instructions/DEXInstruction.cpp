#include "Instructions/DEXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DEXInstruction> registrar("DEXInstruction");
    Instruction::Registrar2<DEXInstruction> registrar2("DEXInstruction");
}

void DEXInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xCA;    
    insctructions.at(opcode) = make_unique<DEXInstruction>(implict, opcode, "DEX", AffectFlags::Negative | AffectFlags::Zero);
}

void DEXInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xCA;    
    auto instruction = DEXInstruction(implict, opcode, "DEX", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(DEXInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t DEXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X--;
    cpu.tick();
    return cpu.X;
}

uint_least16_t DEXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return DEXInstruction::sharedAction(cpu, value);
}