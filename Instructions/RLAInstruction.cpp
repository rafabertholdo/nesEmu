#include "Instructions/RLAInstruction.h"
#include "Instructions/ROLInstruction.h"
#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RLAInstruction> registrar("RLAInstruction");
    Instruction::Registrar2<RLAInstruction> registrar2("RLAInstruction");
}

namespace RLA {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x27,      0x37,     0x2F,      0x3F,      0x3B,      0x23,      0x33};
}

void RLAInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < RLA::opcodeList.size(); i++) {        
        instructions.at(RLA::opcodeList[i]) = make_unique<RLAInstruction>(RLA::addressingModeList[i], RLA::opcodeList[i], "RLA", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void RLAInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < RLA::opcodeList.size(); i++) {
        Instruction instruction(RLA::addressingModeList[i], RLA::opcodeList[i], "RLA", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(RLAInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t RLAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto rolValue = ROLInstruction::sharedAction(cpu, value);
    return ANDInstruction::sharedAction(cpu, rolValue);
}

uint_least16_t RLAInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return RLAInstruction::sharedAction(cpu, value);
}