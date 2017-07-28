#include "Instructions/LDYInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDYInstruction> registrar("LDYInstruction");
    Instruction::Registrar2<LDYInstruction> registrar2("LDYInstruction");
}

namespace LDY {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{              0xA0,     0xA4,      0xB4,     0xAC,      0xBC};
}

void LDYInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < LDY::opcodeList.size(); i++) {        
        instructions.at(LDY::opcodeList[i]) = make_unique<LDYInstruction>(LDY::addressingModeList[i], LDY::opcodeList[i], "LDY", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void LDYInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < LDY::opcodeList.size(); i++) {
        Instruction instruction(LDY::addressingModeList[i], LDY::opcodeList[i], "LDY", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(LDYInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t LDYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.Y = value;
}

uint_least16_t LDYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return LDYInstruction::sharedAction(cpu, value);
}