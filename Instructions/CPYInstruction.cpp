#include "Instructions/CPYInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CPYInstruction> registrar("CPYInstruction");
    Instruction::Registrar2<CPYInstruction> registrar2("CPYInstruction");
}

namespace CPY {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, absolute};
    vector<uint_least8_t> opcodeList{              0xC0,     0xC4,     0xCC};
}

void CPYInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < CPY::opcodeList.size(); i++) {        
        instructions.at(CPY::opcodeList[i]) = make_unique<CPYInstruction>(CPY::addressingModeList[i], CPY::opcodeList[i], "CPY", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void CPYInstruction::createInstructions2(vector<Instruction> &instructions) {    
    for(int i=0; i < CPY::opcodeList.size(); i++) {
        Instruction instruction(CPY::addressingModeList[i], CPY::opcodeList[i], "CPY", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(CPYInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t CPYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = cpu.Y >= value;
    return cpu.Y - value;
}

uint_least16_t CPYInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return CPYInstruction::sharedAction(cpu, value);
}
