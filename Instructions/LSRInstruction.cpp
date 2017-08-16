#include "Instructions/LSRInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LSRInstruction> registrar("LSRInstruction");
    Instruction::Registrar2<LSRInstruction> registrar2("LSRInstruction");
}

namespace LSR {
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x4A,     0x46,      0x56,     0x4E,      0x5E};
}

void LSRInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < LSR::opcodeList.size(); i++) {        
        instructions.at(LSR::opcodeList[i]) = make_unique<LSRInstruction>(LSR::addressingModeList[i], LSR::opcodeList[i], "LSR", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void LSRInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < LSR::opcodeList.size(); i++) {
        Instruction instruction(LSR::addressingModeList[i], LSR::opcodeList[i], "LSR", AffectFlags::Negative | AffectFlags::Zero);

        if (LSR::addressingModeList[i] == accumulator) {
            instruction.setLambda(LSRInstruction::sharedActionA);
        } else {
            instruction.setLambda(LSRInstruction::sharedAction);            
        }        

        instructions.at(instruction.getOpcode()) = instruction;        
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

uint_least16_t LSRInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    if (_addressing == accumulator) {
        return LSRInstruction::sharedActionA(cpu, value);
    } else {
        return LSRInstruction::sharedAction(cpu, value);
    }    
}