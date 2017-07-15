#include "BPLInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BPLInstruction> registrar("BPLInstruction");
}

vector<shared_ptr<Instruction>> BPLInstruction::createInstructions() {
    auto instruction = make_shared<BPLInstruction>(relative,0x10,2,"BPL");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void BPLInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    if (!cpu.Flags.Negative) {
        cpu.PC = value;
    }
}