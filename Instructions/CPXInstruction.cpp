#include "Instructions/CPXInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CPXInstruction> registrar("CPXInstruction");
    Instruction::Registrar2<CPXInstruction> registrar2("CPXInstruction");
}

namespace CPX {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, absolute};
    vector<uint_least8_t> opcodeList{              0xE0,     0xE4,     0xEC};
}

void CPXInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < CPX::opcodeList.size(); i++) {        
        instructions.at(CPX::opcodeList[i]) = make_unique<CPXInstruction>(CPX::addressingModeList[i], CPX::opcodeList[i], "CPX", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void CPXInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < CPX::opcodeList.size(); i++) {
        Instruction instruction(CPX::addressingModeList[i], CPX::opcodeList[i], "CPX", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(CPXInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t CPXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = cpu.X >= value;
    return cpu.X - value;
}

uint_least16_t CPXInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return CPXInstruction::sharedAction(cpu, value);
}