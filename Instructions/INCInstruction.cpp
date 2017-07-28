#include "Instructions/INCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<INCInstruction> registrar("INCInstruction");
    Instruction::Registrar2<INCInstruction> registrar2("INCInstruction");
}

namespace INC {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xE6,      0xF6,     0xEE,      0xFE};
}

void INCInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < INC::opcodeList.size(); i++) {        
        instructions.at(INC::opcodeList[i]) = make_unique<INCInstruction>(INC::addressingModeList[i], INC::opcodeList[i], "INC", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void INCInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < INC::opcodeList.size(); i++) {
        Instruction instruction(INC::addressingModeList[i], INC::opcodeList[i], "INC", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(INCInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t INCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);   
    cpu.tick();     
    cpu.write(value, ++valueFromMemmory);
    return valueFromMemmory;    
}

uint_least16_t INCInstruction::action(CPU& cpu, const uint_least16_t &value) {           
    return INCInstruction::sharedAction(cpu, value);
}