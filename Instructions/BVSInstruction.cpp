#include "Instructions/BVSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BVSInstruction> registrar("BVSInstruction");
}

vector<shared_ptr<Instruction>> BVSInstruction::createInstructions() {
    auto instruction = make_shared<BVSInstruction>(relative,0x70,2,"BVS");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t BVSInstruction::action(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Overflow) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}