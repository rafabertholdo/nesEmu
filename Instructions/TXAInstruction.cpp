#include "Instructions/TXAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TXAInstruction> registrar("TXAInstruction");
    Instruction::Registrar2<TXAInstruction> registrar2("TXAInstruction");
}

void TXAInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x8A;    
    insctructions.at(opcode) = make_unique<TXAInstruction>(implict, opcode, "TXA", AffectFlags::Negative | AffectFlags::Zero);
}

void TXAInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x8A;    
    auto instruction = TXAInstruction(implict, opcode, "TXA", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(TXAInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TXAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.A = cpu.X;
    cpu.tick();
    return cpu.A;
}

uint_least16_t TXAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TXAInstruction::sharedAction(cpu, value);
}