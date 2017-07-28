#include "Instructions/CMPInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CMPInstruction> registrar("CMPInstruction");
    Instruction::Registrar2<CMPInstruction> registrar2("CMPInstruction");
}

namespace CMP {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xC9,     0xC5,      0xD5,     0xCD,      0xDD,      0xD9,      0xC1,      0xD1};    
}

void CMPInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < CMP::opcodeList.size(); i++) {        
        instructions.at(CMP::opcodeList[i]) = make_unique<CMPInstruction>(CMP::addressingModeList[i], CMP::opcodeList[i], "CMP", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void CMPInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < CMP::opcodeList.size(); i++) {
        Instruction instruction(CMP::addressingModeList[i], CMP::opcodeList[i], "CMP", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(CMPInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t CMPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = cpu.A >= value;
    return cpu.A - value;
}

uint_least16_t CMPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return CMPInstruction::sharedAction(cpu, value);
}