#include "Instructions/INYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INYInstruction> registrar("INYInstruction");
}

vector<shared_ptr<Instruction>> INYInstruction::createInstructions() {
    auto instruction = make_shared<INYInstruction>(implict,0xC8,1,"INY", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t INYInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y++;
    cpu.tick();
    return cpu.Y;
}