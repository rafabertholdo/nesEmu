#include "Instructions/LAXInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<LAXInstruction> registrar("LAXInstruction");
}

void LAXInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xA7,      0xB7,     0xAF,      0xBF,      0xA3,      0xB3};

    for(int i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "LAX", LAXInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t LAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = cpu.A = value;
}