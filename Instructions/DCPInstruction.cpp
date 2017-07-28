#include "Instructions/DCPInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<DCPInstruction> registrar("DCPInstruction");
    Instruction::Registrar2<DCPInstruction> registrar2("DCPInstruction");
}

namespace DCP {
    vector<AddressingMode> addressingModeList{zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{             0xC7,      0xD7,     0xCF,      0xDF,      0xDB,      0xC3,      0xD3};
}

void DCPInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < DCP::opcodeList.size(); i++) {        
        instructions.at(DCP::opcodeList[i]) = make_unique<DCPInstruction>(DCP::addressingModeList[i], DCP::opcodeList[i], "DCP", AffectFlags::Negative | AffectFlags::Zero);
    }    
}

void DCPInstruction::createInstructions2(vector<Instruction> &instructions) {    
    for(int i=0; i < DCP::opcodeList.size(); i++) {
        Instruction instruction(DCP::addressingModeList[i], DCP::opcodeList[i], "DCP", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setLambda(DCPInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t DCPInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto valueFromMemmory = cpu.read(value);        
    cpu.write(value, --valueFromMemmory);    
    return cpu.A - valueFromMemmory;
}

uint_least16_t DCPInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return DCPInstruction::sharedAction(cpu, value);
}