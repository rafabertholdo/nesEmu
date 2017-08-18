#include "Instructions/ISBInstruction.h"
#include "Instructions/SBCInstruction.h"
#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ISBInstruction> registrar("ISBInstruction");
}

void ISBInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xE7,      0xF7,     0xEF,      0xFF,      0xFB,      0xE3,      0xF3};    

    for(int i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "ISB", ISBInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);        
    }    
}

uint_least16_t ISBInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto incValue = INCInstruction::sharedAction(cpu, value);    
    return SBCInstruction::sharedAction(cpu, incValue);
}