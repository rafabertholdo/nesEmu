#include "Instructions/STYInstruction.h"
#include "../Utils.cpp"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<STYInstruction> registrar("STYInstruction");
}

void STYInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute};
    vector<uint_least8_t> opcodeList{             0x84,      0x94,     0x8C};

    for(unsigned long i=0; i < opcodeList.size(); i++) {        
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "STY", STYInstruction::sharedAction);        
    }    
}

uint_least16_t STYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.write(value, cpu.Y);
    return cpu.Y;
}
