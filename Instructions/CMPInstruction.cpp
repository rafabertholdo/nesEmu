#include "Instructions/CMPInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CMPInstruction> registrar("CMPInstruction");
}

vector<shared_ptr<Instruction>> CMPInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xC9,     0xC5,      0xD5,     0xCD,      0xDD,      0xD9,      0xC1,      0xD1};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<CMPInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "CMP", AffectFlags::Negative | AffectFlags::Zero | AffectFlags::Carry);
        instruction->readsFromMemory = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t CMPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.A - value;
}

void CMPInstruction::updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Carry = cpu.A >= value;
}