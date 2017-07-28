#include "Instructions/STAInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STAInstruction> registrar("STAInstruction");
    Instruction::Registrar2<STAInstruction> registrar2("STAInstruction");
}

namespace STA {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0x85,      0x95,     0x8D,      0x9D,      0x99,      0x81,      0x91};
}

void STAInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < STA::opcodeList.size(); i++) {        
        instructions.at(STA::opcodeList[i]) = make_unique<STAInstruction>(STA::addressingModeList[i], STA::opcodeList[i], "STA");
    }    
}

void STAInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < STA::opcodeList.size(); i++) {
        Instruction instruction(STA::addressingModeList[i], STA::opcodeList[i], "STA");
        instruction.setLambda(STAInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t STAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.write(value, cpu.A);
    return cpu.A;
}

uint_least16_t STAInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return STAInstruction::sharedAction(cpu, value);
}