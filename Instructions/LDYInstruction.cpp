#include "Instructions/LDYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDYInstruction> registrar("LDYInstruction");
}

vector<shared_ptr<Instruction>> LDYInstruction::createInstructions() {
vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{              0xA0,     0xA4,      0xB4,     0xAC,      0xBC};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<LDYInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "LDY", AffectFlags::Negative | AffectFlags::Zero);
        instruction->readsFromMemory = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t LDYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.Y = value;
}