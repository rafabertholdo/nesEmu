#include "Instructions/BCCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCCInstruction> registrar("BCCInstruction");
}

vector<shared_ptr<Instruction>> BCCInstruction::createInstructions() {
    auto instruction = make_shared<BCCInstruction>(relative,0x90,2,"BCC");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t BCCInstruction::action(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}