#include "Instructions/ASLInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ASLInstruction> registrar("ASLInstruction");
}

vector<shared_ptr<Instruction>> ASLInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x0A,     0x06,      0x16,     0x0E,      0x1E};
    vector<uint_least8_t> lengthList{                   1,        2,         2,        3,         3};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ASLInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ASL", AffectFlags::Negative | AffectFlags::Zero);        
        instructions.push_back(instruction);
    }
    return instructions;
}

uint_least16_t ASLInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b10000000;
    cpu.tick();
    valueFromMemmory = valueFromMemmory << 1;
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t ASLInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    if (dynamic_cast<AccumulatorAddressing*>(_addressing.get())) {
        cpu.Flags.Carry = value & 0b10000000;    
        cpu.A = value << 1;
        cpu.tick();
        return cpu.A;
    } else {
        return ASLInstruction::sharedAction(cpu, value);
    }
}