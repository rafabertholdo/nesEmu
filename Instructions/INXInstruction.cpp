#include "Instructions/INXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INXInstruction> registrar("INXInstruction");
}

vector<shared_ptr<Instruction>> INXInstruction::createInstructions() {
    auto instruction = make_shared<INXInstruction>(implict,0xE8,1,"INX", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t INXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.X++;
    cpu.tick()
    return cpu.X;
}