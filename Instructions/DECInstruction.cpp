#include "Instructions/DECInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DECInstruction> registrar("DECInstruction");
}

vector<shared_ptr<Instruction>> DECInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xC6,      0xD6,     0xCE,      0xDE};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<DECInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "DEC", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t DECInstruction::action(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);        
    cpu.write(value, --valueFromMemmory);
    return valueFromMemmory;    
}