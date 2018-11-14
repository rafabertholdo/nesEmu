#include "Instructions/RRAInstruction.h"
#include "Instructions/RORInstruction.h"
#include "Instructions/ADCInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<RRAInstruction> registrar("RRAInstruction");
}

void RRAInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x67,      0x77,     0x6F,      0x7F,      0x7B,      0x63,      0x73};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "RRA", RRAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t RRAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    auto rorValue = RORInstruction::sharedAction(cpu, value);
    return ADCInstruction::sharedAction(cpu, rorValue);
}
