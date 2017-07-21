#include "Instructions/BCSInstruction.h"
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

uint_least16_t BCSInstruction::action(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}