#include "Instructions/ISBInstruction.h"
#include "Instructions/SBCInstruction.h"
#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ISBInstruction> registrar("ISBInstruction");
    Instruction::Registrar2<ISBInstruction> registrar2("ISBInstruction");
}

namespace ISB {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xE7,      0xF7,     0xEF,      0xFF,      0xFB,      0xE3,      0xF3};    
}

void ISBInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < ISB::opcodeList.size(); i++) {        
        instructions.at(ISB::opcodeList[i]) = make_unique<ISBInstruction>(ISB::addressingModeList[i], ISB::opcodeList[i], "ISB", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void ISBInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < ISB::opcodeList.size(); i++) {
        Instruction instruction(ISB::addressingModeList[i], ISB::opcodeList[i], "ISB", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(ISBInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t ISBInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto incValue = INCInstruction::sharedAction(cpu, value);    
    return SBCInstruction::sharedAction(cpu, incValue);
}

uint_least16_t ISBInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ISBInstruction::sharedAction(cpu, value);
}