#include "CLCInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CLCInstruction> registrar("CLCInstruction");
}

vector<shared_ptr<Instruction>> CLCInstruction::createInstructions() {
    auto instruction = make_shared<CLCInstruction>(implict,0x18,1,"CLC");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void CLCInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    cpu.Flags.Carry = 0;
}