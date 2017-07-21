#include "Instructions/TAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TAXInstruction> registrar("TAXInstruction");
}

vector<shared_ptr<Instruction>> TAXInstruction::createInstructions() {
    auto instruction = make_shared<TAXInstruction>(implict,0xAA,1,"TAX", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TAXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.X = cpu.A;
    cpu.tick();
    return cpu.X;
}