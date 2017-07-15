#include "BVCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVCInstruction> registrar("BVCInstruction");
}

vector<shared_ptr<Instruction>> BVCInstruction::createInstructions() {
    auto instruction = make_shared<BVCInstruction>(relative,0x50,2,"BVC");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void BVCInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    if (!cpu.Flags.Overvlow) {
        cpu.PC = value;
    }
}