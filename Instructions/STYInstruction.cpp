#include "Instructions/STYInstruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<STYInstruction> registrar("STYInstruction");
    Instruction::Registrar2<STYInstruction> registrar2("STYInstruction");
}

namespace STY{
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute};
    vector<uint_least8_t> opcodeList{             0x84,      0x94,     0x8C};
}

void STYInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < STY::opcodeList.size(); i++) {        
        instructions.at(STY::opcodeList[i]) = make_unique<STYInstruction>(STY::addressingModeList[i], STY::opcodeList[i], "STY");
    }    
}

void STYInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < STY::opcodeList.size(); i++) {
        Instruction instruction(STY::addressingModeList[i], STY::opcodeList[i], "STY");
        instruction.setLambda(STYInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t STYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.write(value, cpu.Y);
    return cpu.Y;
}

uint_least16_t STYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return STYInstruction::sharedAction(cpu, value);
}