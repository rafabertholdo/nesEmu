#include "Instructions/LDAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDAInstruction> registrar("LDAInstruction");
    Instruction::Registrar2<LDAInstruction> registrar2("LDAInstruction");
}

namespace LDA {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xA9,     0xA5,      0xB5,     0xAD,      0xBD,      0xB9,      0xA1,      0xB1}; 
}

void LDAInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < LDA::opcodeList.size(); i++) {        
        instructions.at(LDA::opcodeList[i]) = make_unique<LDAInstruction>(LDA::addressingModeList[i], LDA::opcodeList[i], "LDA", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void LDAInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < LDA::opcodeList.size(); i++) {
        Instruction instruction(LDA::addressingModeList[i], LDA::opcodeList[i], "LDA", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(LDAInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t LDAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A = value;
}

uint_least16_t LDAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return LDAInstruction::sharedAction(cpu, value);
}