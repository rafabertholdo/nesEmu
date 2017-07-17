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
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    return cpu.Flags.raw = flagsToPop;
}