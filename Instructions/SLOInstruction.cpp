#include "Instructions/SLOInstruction.h"
#include "Instructions/ASLInstruction.h"
#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SLOInstruction> registrar("SLOInstruction");
    Instruction::Registrar2<SLOInstruction> registrar2("SLOInstruction");
}

namespace SLO {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x07,      0x17,     0x0F,      0x1F,      0x1B,      0x03,      0x13};
}

void SLOInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < SLO::opcodeList.size(); i++) {        
        instructions.at(SLO::opcodeList[i]) = make_unique<SLOInstruction>(SLO::addressingModeList[i], SLO::opcodeList[i], "SLO", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void SLOInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < SLO::opcodeList.size(); i++) {
        Instruction instruction(SLO::addressingModeList[i], SLO::opcodeList[i], "SLO", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(SLOInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t SLOInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = ASLInstruction::sharedAction(cpu, value);
    return ORAInstruction::sharedAction(cpu, aslValue);
}

uint_least16_t SLOInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return SLOInstruction::sharedAction(cpu, value);
}