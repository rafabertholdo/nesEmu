#include "Instructions/ROLInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ROLInstruction> registrar("ROLInstruction");
    Instruction::Registrar2<ROLInstruction> registrar2("ROLInstruction");
}

namespace ROL {
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x2A,     0x26,      0x36,     0x2E,      0x3E};
}

void ROLInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < ROL::opcodeList.size(); i++) {        
        instructions.at(ROL::opcodeList[i]) = make_unique<ROLInstruction>(ROL::addressingModeList[i], ROL::opcodeList[i], "ROL", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void ROLInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < ROL::opcodeList.size(); i++) {
        Instruction instruction(ROL::addressingModeList[i], ROL::opcodeList[i], "ROL", AffectFlags::Negative | AffectFlags::Zero);
        if (ROL::addressingModeList[i] == accumulator) {
            instruction.setLambda(ROLInstruction::sharedActionA);
        } else {
            instruction.setLambda(ROLInstruction::sharedAction);            
        }        
        instructions.at(instruction.getOpcode()) = instruction;        
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

uint_least16_t ROLInstruction::action(CPU& cpu, const uint_least16_t &value) {            
    if (_addressing == accumulator) {        
        return ROLInstruction::sharedActionA(cpu, value);
    } else {
        return ROLInstruction::sharedAction(cpu, value);
    }    
}