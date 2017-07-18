#include "Instructions/TYAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TYAInstruction> registrar("TYAInstruction");
}

vector<shared_ptr<Instruction>> TYAInstruction::createInstructions() {
    auto instruction = make_shared<TYAInstruction>(implict,0x98,1,"TYA", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TYAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return cpu.A = cpu.Y;
}