#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ORAInstruction> registrar("ORAInstruction");
    Instruction::Registrar2<ORAInstruction> registrar2("ORAInstruction");
}

namespace ORA {
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x09,     0x05,      0x15,     0x0D,      0x1D,      0x19,      0x01,      0x11};
}

void ORAInstruction::createInstructions(vector<unique_ptr<Instruction>> &instructions) {
    for(int i=0; i < ORA::opcodeList.size(); i++) {        
        instructions.at(ORA::opcodeList[i]) = make_unique<ORAInstruction>(ORA::addressingModeList[i], ORA::opcodeList[i], "ORA", AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

void ORAInstruction::createInstructions2(vector<Instruction> &instructions) {    

    for(int i=0; i < ORA::opcodeList.size(); i++) {
        Instruction instruction(ORA::addressingModeList[i], ORA::opcodeList[i], "ORA", AffectFlags::Negative | AffectFlags::Zero, true);
        instruction.setLambda(ORAInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}

uint_least16_t ORAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A |= value;
}

uint_least16_t ORAInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ORAInstruction::sharedAction(cpu, value);
}