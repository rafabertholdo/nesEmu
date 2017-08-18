#include "Instructions/STXInstruction.h"
#include "../Utils.cpp"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<STXInstruction> registrar("STXInstruction");
}

void STXInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute};
    vector<uint_least8_t> opcodeList{             0x86,      0x96,     0x8E};

    for(int i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "STX", STXInstruction::sharedAction);
    }    
}

uint_least16_t STXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.write(value, cpu.X);    
    return cpu.X;
}