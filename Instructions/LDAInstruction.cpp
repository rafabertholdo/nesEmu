#include "LDAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

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
    cpu.A = value;
    cout << " ";
    Utils<uint_least8_t>::printHex(cpu.A);
    cout << std::endl;
}