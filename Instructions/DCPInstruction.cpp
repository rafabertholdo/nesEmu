#include "Instructions/DCPInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DCPInstruction> registrar("DCPInstruction");
}

void DCPInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xC7,      0xD7,     0xCF,      0xDF,      0xDB,      0xC3,      0xD3};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "DCP", DCPInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero);
    }    
}

uint_least16_t DCPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);        
    cpu.write(value, --valueFromMemmory);    
    return cpu.A - valueFromMemmory;
}
