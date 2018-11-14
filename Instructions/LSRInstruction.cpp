#include "Instructions/LSRInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<LSRInstruction> registrar("LSRInstruction");
}

void LSRInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x4A,     0x46,      0x56,     0x4E,      0x5E};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        auto functionPointer = addressingModeList[i] == accumulator ? LSRInstruction::sharedActionA : LSRInstruction::sharedAction;
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "LSR", functionPointer, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t LSRInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b1;
    valueFromMemmory = valueFromMemmory >> 1;
    cpu.tick();
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t LSRInstruction::sharedActionA(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = value & 0b1;    
    cpu.A = value >> 1;
    cpu.tick();
    return cpu.A;
}
