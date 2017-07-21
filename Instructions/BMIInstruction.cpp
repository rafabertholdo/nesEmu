#include "Instructions/BMIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BMIInstruction> registrar("BMIInstruction");
}

vector<shared_ptr<Instruction>> BMIInstruction::createInstructions() {
    auto instruction = make_shared<BMIInstruction>(relative,0x30,2,"BMI");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t BMIInstruction::action(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Negative) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}