#include "Instructions/LDAInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<LDAInstruction> registrar("LDAInstruction");
}

void LDAInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xA9,     0xA5,      0xB5,     0xAD,      0xBD,      0xB9,      0xA1,      0xB1}; 

    for(int i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "LDA", LDAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t LDAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A = value;
}