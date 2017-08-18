#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ANDInstruction> registrar("ANDInstruction");    
}

void ANDInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x29,     0x25,      0x35,     0x2D,      0x3D,      0x39,      0x21,      0x31};

    for(int i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "AND", ANDInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t ANDInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) { 
    return cpu.A &= value;
}