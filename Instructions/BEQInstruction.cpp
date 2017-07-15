#include "BEQInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BEQInstruction> registrar("BEQInstruction");
}

vector<shared_ptr<Instruction>> BEQInstruction::createInstructions() {
    auto instruction = make_shared<BEQInstruction>(relative,0xF0,2,"BEQ");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void BEQInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    if (cpu.Flags.Zero) {
        cpu.PC = value;
    }
}