#include "Instructions/CMPInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CMPInstruction> registrar("CMPInstruction");
}

void CMPInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xC9,     0xC5,      0xD5,     0xCD,      0xDD,      0xD9,      0xC1,      0xD1};    
    
    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "CMP", CMPInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t CMPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = cpu.A >= value;
    return cpu.A - value;
}
