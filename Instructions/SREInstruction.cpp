#include "Instructions/SREInstruction.h"
#include "Instructions/LSRInstruction.h"
#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SREInstruction> registrar("SREInstruction");
    Instruction::Registrar2<SREInstruction> registrar2("SREInstruction");
}

namespace SRE {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x47,      0x57,     0x4F,      0x5F,      0x5B,      0x43,      0x53};
}

void SREInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < SRE::opcodeList.size(); i++) {        
        instructions.at(SRE::opcodeList[i]) = make_unique<SREInstruction>(SRE::addressingModeList[i], SRE::opcodeList[i], "SRE", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void SREInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < SRE::opcodeList.size(); i++) {
        Instruction instruction(SRE::addressingModeList[i], SRE::opcodeList[i], "SRE", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(SREInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t SREInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = LSRInstruction::sharedAction(cpu, value);
    return EORInstruction::sharedAction(cpu, aslValue);
}

uint_least16_t SREInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return SREInstruction::sharedAction(cpu, value);
}