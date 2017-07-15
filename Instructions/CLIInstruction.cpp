#include "CLIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLIInstruction> registrar("CLIInstruction");
}

vector<shared_ptr<Instruction>> CLIInstruction::createInstructions() {
    auto instruction = make_shared<CLIInstruction>(implict,0x58,1,"CLI");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void CLIInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    cpu.Flags.InterruptEnabled = 0;
}