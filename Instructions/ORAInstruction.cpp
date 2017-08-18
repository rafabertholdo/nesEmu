#include "Instructions/ORAInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<ORAInstruction> registrar("ORAInstruction");
}

void ORAInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x09,     0x05,      0x15,     0x0D,      0x1D,      0x19,      0x01,      0x11};
    
    for(int i=0; i < opcodeList.size(); i++) {
        Instruction instruction(addressingModeList[i], opcodeList[i], "ORA", ORAInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
        instructions[opcodeList[i]] = instruction;        
    }    
}

uint_least16_t ORAInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A |= value;
}