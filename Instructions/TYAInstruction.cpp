#include "Instructions/TYAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TYAInstruction> registrar("TYAInstruction");
    Instruction::Registrar2<TYAInstruction> registrar2("TYAInstruction");
}

void TYAInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x98;    
    insctructions.at(opcode) = make_unique<TYAInstruction>(implict, opcode, "TYA", AffectFlags::Negative | AffectFlags::Zero);
}

void TYAInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x98;    
    auto instruction = TYAInstruction(implict, opcode, "TYA", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(TYAInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TYAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.A = cpu.Y;
    cpu.tick();
    return cpu.A;
}

uint_least16_t TYAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TYAInstruction::sharedAction(cpu, value);
}