#include "Instructions/STAInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STAInstruction> registrar("STAInstruction");
}

vector<shared_ptr<Instruction>> STAInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x85,      0x95,     0x8D,      0x9D,      0x99,      0x81,      0x91};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<STAInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "STA");
        instruction->printsActionValue = true;
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t STAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    cpu.write(value, cpu.A);
    return cpu.A;
}