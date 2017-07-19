#include "Instructions/RORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RORInstruction> registrar("RORInstruction");
}

vector<shared_ptr<Instruction>> RORInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x6A,     0x66,      0x76,     0x6E,      0x7E};
    vector<uint_least8_t> lengthList{                   1,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<RORInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ROR", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t RORInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    bool carry = cpu.Flags.Carry;
    if (dynamic_cast<AccumulatorAddressing*>(addressing.get())) {        
        cpu.Flags.Carry = value & 0b1;    
        return cpu.A = (value >> 1) + (carry << 7);
    } else {
        auto valueFromMemmory = cpu.read(value);
        cpu.Flags.Carry = valueFromMemmory & 0b1;
        valueFromMemmory = (valueFromMemmory >> 1) + (carry << 7);
        cpu.write(value, valueFromMemmory);
        return valueFromMemmory;
    }    
}