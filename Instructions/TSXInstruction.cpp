#include "Instructions/TSXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TSXInstruction> registrar("TSXInstruction");
}

vector<shared_ptr<Instruction>> TSXInstruction::createInstructions() {
    auto instruction = make_shared<TSXInstruction>(implict,0xBA,1,"TSX", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TSXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return cpu.X = cpu.SP;
}