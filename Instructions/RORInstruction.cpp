#include "Instructions/RORInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<RORInstruction> registrar("RORInstruction");
}

void RORInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x6A,     0x66,      0x76,     0x6E,      0x7E};

    for(int i=0; i < opcodeList.size(); i++) {
        auto functionPointer = addressingModeList[i] == accumulator ? RORInstruction::sharedActionA : RORInstruction::sharedAction;
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "ROR", functionPointer, AffectFlags::Negative | AffectFlags::Zero);        
    }    
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