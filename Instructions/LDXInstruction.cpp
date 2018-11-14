#include "Instructions/LDXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<LDXInstruction> registrar("LDXInstruction");
}

void LDXInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageY, absolute, absoluteY};
    vector<uint_least8_t> opcodeList{              0xA2,     0xA6,      0xB6,     0xAE,      0xBE};

    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "LDX", LDXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t LDXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = value;
}
