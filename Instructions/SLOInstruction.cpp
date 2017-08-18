#include "Instructions/SLOInstruction.h"
#include "Instructions/ASLInstruction.h"
#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SLOInstruction> registrar("SLOInstruction");
}

void SLOInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x07,      0x17,     0x0F,      0x1F,      0x1B,      0x03,      0x13};

    for(int i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "SLO", SLOInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
    }    
}

uint_least16_t SLOInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto aslValue = ASLInstruction::sharedAction(cpu, value);
    return ORAInstruction::sharedAction(cpu, aslValue);
}