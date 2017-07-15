#include "Instructions/PLPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<PLPInstruction> registrar("PLPInstruction");
}

vector<shared_ptr<Instruction>> PLPInstruction::createInstructions() {
    auto instruction = make_shared<PLPInstruction>(implict,0x28,1,"PLP");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t PLPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.Flags.raw = cpu.pop();
}