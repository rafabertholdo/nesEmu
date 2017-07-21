#include "Instructions/DEYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DEYInstruction> registrar("DEYInstruction");
}

vector<shared_ptr<Instruction>> DEYInstruction::createInstructions() {
    auto instruction = make_shared<DEYInstruction>(implict,0x88,1,"DEY", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t DEYInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.Y--;
    cpu.tick();
    return cpu.Y;
}