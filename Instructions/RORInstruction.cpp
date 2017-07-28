#include "Instructions/RORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RORInstruction> registrar("RORInstruction");
    Instruction::Registrar2<RORInstruction> registrar2("RORInstruction");
}

namespace ROR {
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x6A,     0x66,      0x76,     0x6E,      0x7E};
}

void RORInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < ROR::opcodeList.size(); i++) {        
        instructions.at(ROR::opcodeList[i]) = make_unique<RORInstruction>(ROR::addressingModeList[i], ROR::opcodeList[i], "ROR", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void RORInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < ROR::opcodeList.size(); i++) {
        Instruction instruction(ROR::addressingModeList[i], ROR::opcodeList[i], "ROR", AffectFlags::Negative | AffectFlags::Zero);
        if (ROR::addressingModeList[i] == accumulator) {
            instruction.setLambda(RORInstruction::sharedActionA);
        } else {
            instruction.setLambda(RORInstruction::sharedAction);            
        }        
        instructions.at(instruction.getOpcode()) = instruction;        
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

uint_least16_t RORInstruction::action(CPU& cpu, const uint_least16_t &value) {        

    if (dynamic_cast<AccumulatorAddressing*>(_addressing.get())) {        
        return RORInstruction::sharedActionA(cpu, value);
    } else {
        return RORInstruction::sharedAction(cpu, value);
    }    
}