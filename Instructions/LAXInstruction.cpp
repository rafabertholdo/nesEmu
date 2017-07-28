#include "Instructions/LAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<LAXInstruction> registrar("LAXInstruction");
    Instruction::Registrar2<LAXInstruction> registrar2("LAXInstruction");
}

namespace LAX {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageY, absolute, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xA7,      0xB7,     0xAF,      0xBF,      0xA3,      0xB3};
}

void LAXInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < LAX::opcodeList.size(); i++) {        
        instructions.at(LAX::opcodeList[i]) = make_unique<LAXInstruction>(LAX::addressingModeList[i], LAX::opcodeList[i], "LAX", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void LAXInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < LAX::opcodeList.size(); i++) {
        Instruction instruction(LAX::addressingModeList[i], LAX::opcodeList[i], "LAX", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(LAXInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t LAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.X = cpu.A = value;
}

uint_least16_t LAXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return LAXInstruction::sharedAction(cpu, value);
}