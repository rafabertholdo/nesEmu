#include "Instructions/ANDInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ANDInstruction> registrar("ANDInstruction");
    Instruction::Registrar2<ANDInstruction> registrar2("ANDInstruction");
}

namespace AND {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x29,     0x25,      0x35,     0x2D,      0x3D,      0x39,      0x21,      0x31};
}

void ANDInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < AND::opcodeList.size(); i++) {        
        instructions.at(AND::opcodeList[i]) = make_unique<ANDInstruction>(AND::addressingModeList[i], AND::opcodeList[i], "AND", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void ANDInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < AND::opcodeList.size(); i++) {
        Instruction instruction(AND::addressingModeList[i], AND::opcodeList[i], "AND", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(ANDInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t ANDInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) { 
    return cpu.A &= value;
}

uint_least16_t ANDInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ANDInstruction::sharedAction(cpu, value);
}