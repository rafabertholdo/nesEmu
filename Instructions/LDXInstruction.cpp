#include "Instructions/LDXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDXInstruction> registrar("LDXInstruction");
}

vector<shared_ptr<Instruction>> LDXInstruction::createInstructions() {
vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageY, absolute, absoluteY};
    vector<uint_least8_t> opcodeList{              0xA2,     0xA6,      0xB6,     0xAE,      0xBE};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<LDXInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "LDX", AffectFlags::Negative | AffectFlags::Zero);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t LDXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = value;
}