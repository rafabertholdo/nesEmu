#include "Instructions/RTSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RTSInstruction> registrar("RTSInstruction");
}

vector<shared_ptr<Instruction>> RTSInstruction::createInstructions() {
    auto instruction = make_shared<RTSInstruction>(implict,0x60,1,"RTS");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t RTSInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.PC = cpu.pop() + (cpu.pop() << 8) + 1;
}