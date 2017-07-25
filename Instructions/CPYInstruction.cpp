#include "Instructions/CPYInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CPYInstruction> registrar("CPYInstruction");
}

vector<shared_ptr<Instruction>> CPYInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, absolute};
    vector<uint_least8_t> opcodeList{              0xC0,     0xC4,     0xCC};
    vector<uint_least8_t> lengthList{                 2,        2,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<CPYInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "CPY", AffectFlags::Negative | AffectFlags::Zero | AffectFlags::Carry);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t CPYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.Y - value;
}

void CPYInstruction::updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Carry = cpu.Y >= value;
}