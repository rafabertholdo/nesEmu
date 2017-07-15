#include "LDAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDAInstruction> registrar("LDAInstruction");
}

vector<shared_ptr<Instruction>> LDAInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xA9,     0xA5,      0xB5,     0xAD,      0xBD,      0xB9,      0xA1,      0xB1};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<LDAInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "LDA");
        instructions.push_back(instruction);
    }
    return instructions;
}

void LDAInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    cpu.Flags.Zero = value == 0;
    cpu.Flags.Negative = static_cast<int_least16_t>(value) < 0;
    cpu.A = value;
}