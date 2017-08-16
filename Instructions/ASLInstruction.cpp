#include "Instructions/ASLInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ASLInstruction> registrar("ASLInstruction");
    Instruction::Registrar2<ASLInstruction> registrar2("ASLInstruction");
}

namespace ASL {
    vector<AddressingMode> addressingModeList{accumulator, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{                0x0A,     0x06,      0x16,     0x0E,      0x1E};
}

void ASLInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < ASL::opcodeList.size(); i++) {        
        instructions.at(ASL::opcodeList[i]) = make_unique<ASLInstruction>(ASL::addressingModeList[i], ASL::opcodeList[i], "ASL", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void ASLInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < ASL::opcodeList.size(); i++) {
        Instruction instruction(ASL::addressingModeList[i], ASL::opcodeList[i], "ASL", AffectFlags::Negative | AffectFlags::Zero);

        if (ASL::addressingModeList[i] == accumulator) {
            instruction.setLambda(ASLInstruction::sharedActionA);
        } else {
            instruction.setLambda(ASLInstruction::sharedAction);            
        }        
        instructions.at(instruction.getOpcode()) = instruction;        
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

uint_least16_t ASLInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    if (_addressing == accumulator) {
        return ASLInstruction::sharedActionA(cpu, value);
    } else {
        return ASLInstruction::sharedAction(cpu, value);
    }
}