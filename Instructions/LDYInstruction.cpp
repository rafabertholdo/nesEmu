#include "Instructions/LDYInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<LDYInstruction> registrar("LDYInstruction");
}

void LDYInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{              0xA0,     0xA4,      0xB4,     0xAC,      0xBC};

    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "LDY", LDYInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        ;        
    }    
}

uint_least16_t LDYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.Y = value;
}
