#include "Instructions/LSRInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LSRInstruction> registrar("LSRInstruction");
}

vector<shared_ptr<Instruction>> LSRInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x4A,     0x46,      0x56,     0x4E,      0x5E};
    vector<uint_least8_t> lengthList{                   1,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<LSRInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "LSR", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t LSRInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b1;
    valueFromMemmory = valueFromMemmory >> 1;
    cpu.Tick();
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t LSRInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    if (dynamic_cast<AccumulatorAddressing*>(addressing.get())) {
        cpu.Flags.Carry = value & 0b1;    
        cpu.A = value >> 1;
        cpu.Tick();
        return cpu.A;
    } else {
        return LSRInstruction::sharedAction(cpu, value);
    }    
}