#include "Instructions/CPXInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CPXInstruction> registrar("CPXInstruction");
}

vector<shared_ptr<Instruction>> CPXInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, absolute};
    vector<uint_least8_t> opcodeList{              0xE0,     0xE4,     0xEC};
    vector<uint_least8_t> lengthList{                 2,        2,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<CPXInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "CPX", AffectFlags::Negative | AffectFlags::Zero | AffectFlags::Carry);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t CPXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.X - value;
}

void CPXInstruction::updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Carry = cpu.X >= value;
}