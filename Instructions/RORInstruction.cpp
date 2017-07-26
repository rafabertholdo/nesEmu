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

uint_least16_t RORInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    bool carry = cpu.Flags.Carry;
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b1;
    valueFromMemmory = (valueFromMemmory >> 1) + (carry << 7);
    cpu.tick();
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t RORInstruction::sharedActionA(CPU& cpu, const uint_least16_t &value) {
    bool carry = cpu.Flags.Carry;
    cpu.Flags.Carry = value & 0b1;    
    cpu.A = (value >> 1) + (carry << 7);
    cpu.tick();
    return cpu.A;
}

uint_least16_t RORInstruction::action(CPU& cpu, const uint_least16_t &value) {        

    if (dynamic_cast<AccumulatorAddressing*>(_addressing.get())) {        
        return RORInstruction::sharedActionA(cpu, value);
    } else {
        return RORInstruction::sharedAction(cpu, value);
    }    
}