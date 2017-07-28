#include "Instructions/STXInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STXInstruction> registrar("STXInstruction");
    Instruction::Registrar2<STXInstruction> registrar2("STXInstruction");
}

namespace STX {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute};
    vector<uint_least8_t> opcodeList{             0x86,      0x96,     0x8E};
}

void STXInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < STX::opcodeList.size(); i++) {        
        instructions.at(STX::opcodeList[i]) = make_unique<STXInstruction>(STX::addressingModeList[i], STX::opcodeList[i], "STX");
    }    
}

void STXInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < STX::opcodeList.size(); i++) {
        Instruction instruction(STX::addressingModeList[i], STX::opcodeList[i], "STX");
        instruction.setLambda(STXInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t STXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.write(value, cpu.X);    
    return cpu.X;
}

uint_least16_t STXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return STXInstruction::sharedAction(cpu, value);
}