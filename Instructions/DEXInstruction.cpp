#include "Instructions/DEXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DEXInstruction> registrar("DEXInstruction");
}

vector<shared_ptr<Instruction>> DEXInstruction::createInstructions() {
    auto instruction = make_shared<DEXInstruction>(implict,0xCA,1,"DEX", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t DEXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.X--;
    cpu.tick();
    return cpu.X;
}