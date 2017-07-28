#include "Instructions/RRAInstruction.h"
#include "Instructions/RORInstruction.h"
#include "Instructions/ADCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RRAInstruction> registrar("RRAInstruction");
    Instruction::Registrar2<RRAInstruction> registrar2("RRAInstruction");
}

namespace RRA {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x67,      0x77,     0x6F,      0x7F,      0x7B,      0x63,      0x73};
}

void RRAInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < RRA::opcodeList.size(); i++) {        
        instructions.at(RRA::opcodeList[i]) = make_unique<RRAInstruction>(RRA::addressingModeList[i], RRA::opcodeList[i], "RRA", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void RRAInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < RRA::opcodeList.size(); i++) {
        Instruction instruction(RRA::addressingModeList[i], RRA::opcodeList[i], "RRA", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(RRAInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t RRAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto rorValue = RORInstruction::sharedAction(cpu, value);
    return ADCInstruction::sharedAction(cpu, rorValue);
}

uint_least16_t RRAInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return RRAInstruction::sharedAction(cpu, value);
}