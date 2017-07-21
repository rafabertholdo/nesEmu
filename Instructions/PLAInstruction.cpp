#include "Instructions/PLAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PLAInstruction> registrar("PLAInstruction");
}

vector<shared_ptr<Instruction>> PLAInstruction::createInstructions() {
    auto instruction = make_shared<PLAInstruction>(implict,0x68,1,"PLA", AffectFlags::Negative | AffectFlags::Zero);
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t PLAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    cpu.tick();
    return cpu.A = cpu.pop();
}