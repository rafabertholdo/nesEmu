#include "Instructions/DECInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DECInstruction> registrar("DECInstruction");
    Instruction::Registrar2<DECInstruction> registrar2("DECInstruction");
}

namespace DEC {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX};
    vector<uint_least8_t> opcodeList{             0xC6,      0xD6,     0xCE,      0xDE};
}

void DECInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < DEC::opcodeList.size(); i++) {        
        instructions.at(DEC::opcodeList[i]) = make_unique<DECInstruction>(DEC::addressingModeList[i], DEC::opcodeList[i], "DEC", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void DECInstruction::createInstructions2(vector<Instruction> &instructions) {    
    for(int i=0; i < DEC::opcodeList.size(); i++) {
        Instruction instruction(DEC::addressingModeList[i], DEC::opcodeList[i], "DEC", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(DECInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t DECInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {           
    auto valueFromMemmory = cpu.read(value);        
    cpu.tick();
    cpu.write(value, --valueFromMemmory);
    return valueFromMemmory;    
}

uint_least16_t DECInstruction::action(CPU& cpu, const uint_least16_t &value) {           
    return DECInstruction::sharedAction(cpu, value);
}