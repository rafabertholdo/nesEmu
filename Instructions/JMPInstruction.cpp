#include "Instructions/JMPInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<JMPInstruction> registrar("JMPInstruction");
}

void JMPInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{absolute, indirect};
    vector<uint_least8_t> opcodeList{             0x4C,     0x6C};

    for(int i=0; i < opcodeList.size(); i++) {                
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "JMP", JMPInstruction::sharedAction);
    }    
}

uint_least16_t JMPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.PC = value;
}