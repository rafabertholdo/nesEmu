#include "Instructions/LDXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LDXInstruction> registrar("LDXInstruction");
    Instruction::Registrar2<LDXInstruction> registrar2("LDXInstruction");
}

namespace LDX {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageY, absolute, absoluteY};
    vector<uint_least8_t> opcodeList{              0xA2,     0xA6,      0xB6,     0xAE,      0xBE};
}

void LDXInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < LDX::opcodeList.size(); i++) {        
        instructions.at(LDX::opcodeList[i]) = make_unique<LDXInstruction>(LDX::addressingModeList[i], LDX::opcodeList[i], "LDX", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void LDXInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < LDX::opcodeList.size(); i++) {
        Instruction instruction(LDX::addressingModeList[i], LDX::opcodeList[i], "LDX", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(LDXInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t LDXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = value;
}

uint_least16_t LDXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return LDXInstruction::sharedAction(cpu, value);
}