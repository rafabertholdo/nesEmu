#include "Instructions/DCPInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DCPInstruction> registrar("DCPInstruction");
}

vector<shared_ptr<Instruction>> DCPInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xC7,      0xD7,     0xCF,      0xDF,      0xDB,      0xC3,      0xD3};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<DCPInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "DCP", AffectFlags::Negative | AffectFlags::Zero);
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t DCPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);        
    cpu.write(value, --valueFromMemmory);    
    return cpu.A - valueFromMemmory;
}