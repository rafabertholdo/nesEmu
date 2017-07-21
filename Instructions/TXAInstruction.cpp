#include "Instructions/TXAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TXAInstruction> registrar("TXAInstruction");
}

vector<shared_ptr<Instruction>> TXAInstruction::createInstructions() {
    auto instruction = make_shared<TXAInstruction>(implict,0x8A,1,"TXA", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TXAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.A = cpu.X
    cpu.tick();
    return cpu.A;
}