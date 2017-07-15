#include "BCSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCSInstruction> registrar("BCSInstruction");
}

vector<shared_ptr<Instruction>> BCSInstruction::createInstructions() {
    auto instruction = make_shared<BCSInstruction>(relative,0xB0,2,"BCS");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void BCSInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    if (cpu.Flags.Carry) {
        cpu.PC = value;
    }
}