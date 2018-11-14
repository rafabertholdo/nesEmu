#include "Instructions/ASLInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ASLInstruction> registrar("ASLInstruction");    
}

void ASLInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x0A,     0x06,      0x16,     0x0E,      0x1E};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        actionFunctionPointer_t actionFP = addressingModeList[i] == accumulator ? actionFP = ASLInstruction::sharedActionA : ASLInstruction::sharedAction;        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "ASL", actionFP, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t ASLInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);
    cpu.Flags.Carry = valueFromMemmory & 0b10000000;
    cpu.tick();
    valueFromMemmory = valueFromMemmory << 1;
    cpu.write(value, valueFromMemmory);
    return valueFromMemmory;
}

uint_least16_t ASLInstruction::sharedActionA(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = value & 0b10000000;    
    cpu.A = value << 1;
    cpu.tick();
    return cpu.A;
}
