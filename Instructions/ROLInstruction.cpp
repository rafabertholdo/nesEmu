#include "Instructions/ROLInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<ROLInstruction> registrar("ROLInstruction");
}

void ROLInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x2A,     0x26,      0x36,     0x2E,      0x3E};

    for(int i=0; i < opcodeList.size(); i++) {
        auto functionPointer = addressingModeList[i] == accumulator ? ROLInstruction::sharedActionA : ROLInstruction::sharedAction;
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "ROL", functionPointer, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t ROLInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    bool carry = cpu.Flags.Carry;
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b10000000;
    valueFromMemmory = ((valueFromMemmory << 1)% 0x100) + carry;
    cpu.tick();
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t ROLInstruction::sharedActionA(CPU& cpu, const uint_least16_t &value) {
    bool carry = cpu.Flags.Carry;
    cpu.Flags.Carry = value & 0b10000000;    
    cpu.A = ((value << 1) % 0x100) + carry;
    cpu.tick();
    return cpu.A;
}
