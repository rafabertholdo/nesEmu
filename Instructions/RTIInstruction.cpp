#include "Instructions/RTIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RTIInstruction> registrar("RTIInstruction");
}

vector<shared_ptr<Instruction>> RTIInstruction::createInstructions() {
    auto instruction = make_shared<RTIInstruction>(implict,0x40,1,"RTI");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t RTIInstruction::action(CPU& cpu, const uint_least16_t &value) {
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    cpu.Flags.raw = flagsToPop;
    //cpu.Flags.InterruptDisabled = 0;
    return cpu.PC = cpu.pop() + (cpu.pop() << 8);
}