#include "Instructions/RLAInstruction.h"
#include "Instructions/ROLInstruction.h"
#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<RLAInstruction> registrar("RLAInstruction");
}

void RLAInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x27,      0x37,     0x2F,      0x3F,      0x3B,      0x23,      0x33};

    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "RLA", RLAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t RLAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto rolValue = ROLInstruction::sharedAction(cpu, value);
    return ANDInstruction::sharedAction(cpu, rolValue);
}
