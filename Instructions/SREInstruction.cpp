#include "Instructions/SREInstruction.h"
#include "Instructions/LSRInstruction.h"
#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SREInstruction> registrar("SREInstruction");
}

void SREInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x47,      0x57,     0x4F,      0x5F,      0x5B,      0x43,      0x53};

    for(int i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "SRE", SREInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
    }    
}

uint_least16_t SREInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = LSRInstruction::sharedAction(cpu, value);
    return EORInstruction::sharedAction(cpu, aslValue);
}