#include "Instructions/STAInstruction.h"
#include "../Utils.cpp"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<STAInstruction> registrar("STAInstruction");
}

void STAInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x85,      0x95,     0x8D,      0x9D,      0x99,      0x81,      0x91};

    for(int i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "STA", STAInstruction::sharedAction);        
    }    
}

uint_least16_t STAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.write(value, cpu.A);
    return cpu.A;
}