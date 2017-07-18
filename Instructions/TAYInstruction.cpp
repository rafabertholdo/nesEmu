#include "Instructions/TAYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TAYInstruction> registrar("TAYInstruction");
}

vector<shared_ptr<Instruction>> TAYInstruction::createInstructions() {
    auto instruction = make_shared<TAYInstruction>(implict,0xA8,1,"TAY", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TAYInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return cpu.Y = cpu.A;
}