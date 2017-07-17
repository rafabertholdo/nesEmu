#include "Instructions/PHAInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PHAInstruction> registrar("PHAInstruction");
}

vector<shared_ptr<Instruction>> PHAInstruction::createInstructions() {
    auto instruction = make_shared<PHAInstruction>(implict,0x48,1,"PHA");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t PHAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    cpu.push(cpu.A);
    return cpu.A;
}