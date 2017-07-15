#include "BNEInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BNEInstruction> registrar("BNEInstruction");
}

vector<shared_ptr<Instruction>> BNEInstruction::createInstructions() {
    auto instruction = make_shared<BNEInstruction>(relative,0xD0,2,"BNE");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void BNEInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    if (!cpu.Flags.Zero) {
        cpu.PC = value;
    }
}