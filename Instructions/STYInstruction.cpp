#include "STYInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STYInstruction> registrar("STYInstruction");
}

vector<shared_ptr<Instruction>> STYInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute};
    vector<uint_least8_t> opcodeList{             0x84,      0x94,     0x8C};
    vector<uint_least8_t> lengthList{                2,         2,        3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<STYInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "STX");
        instructions.push_back(instruction);
    }
    return instructions;
}

void STYInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);    
    cpu.write(value, cpu.Y);
}