#include "Instructions/JMPInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<JMPInstruction> registrar("JMPInstruction");
    Instruction::Registrar2<JMPInstruction> registrar2("JMPInstruction");
}

namespace JMP {
    vector<AddressingMode> addressingModeList{absolute, indirect};
    vector<uint_least8_t> opcodeList{             0x4C,     0x6C};
}

void JMPInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < JMP::opcodeList.size(); i++) {        
        instructions.at(JMP::opcodeList[i]) = make_unique<JMPInstruction>(JMP::addressingModeList[i], JMP::opcodeList[i], "JMP");
    }    
}

void JMPInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < JMP::opcodeList.size(); i++) {
        Instruction instruction(JMP::addressingModeList[i], JMP::opcodeList[i], "JMP");
        instruction.setLambda(JMPInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t JMPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.PC = value;
}

uint_least16_t JMPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return JMPInstruction::sharedAction(cpu, value);
}