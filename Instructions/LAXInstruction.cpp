#include "Instructions/LAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LAXInstruction> registrar("LAXInstruction");
}

vector<shared_ptr<Instruction>> LAXInstruction::createInstructions() {
vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xA7,      0xB7,     0xAF,      0xBF,      0xA3,      0xB3};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<LAXInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "LAX", AffectFlags::Negative | AffectFlags::Zero);
        instruction->readsFromMemory = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t LAXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = cpu.A = value;
}