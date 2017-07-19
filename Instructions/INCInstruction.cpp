#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INCInstruction> registrar("INCInstruction");
}

vector<shared_ptr<Instruction>> INCInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xE6,      0xF6,     0xEE,      0xFE};
    vector<uint_least8_t> lengthList{                2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<INCInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "INC", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t INCInstruction::action(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);        
    cpu.write(value, ++valueFromMemmory);
    return valueFromMemmory;    
}