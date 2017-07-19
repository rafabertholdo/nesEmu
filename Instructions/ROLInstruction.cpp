#include "Instructions/ROLInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ROLInstruction> registrar("ROLInstruction");
}

vector<shared_ptr<Instruction>> ROLInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x2A,     0x26,      0x36,     0x2E,      0x3E};
    vector<uint_least8_t> lengthList{                   1,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ROLInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ROL", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t ROLInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    bool carry = cpu.Flags.Carry;
    if (dynamic_cast<AccumulatorAddressing*>(addressing.get())) {        
        cpu.Flags.Carry = value & 0b10000000;    
        return cpu.A = ((value << 1) % 0x100) + carry;
    } else {
        auto valueFromMemmory = cpu.read(value);
        cpu.Flags.Carry = valueFromMemmory & 0b10000000;
        valueFromMemmory = ((valueFromMemmory << 1)% 0x100) + carry;
        cpu.write(value, valueFromMemmory);
        return valueFromMemmory;
    }    
}