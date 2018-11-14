#include "Instructions/CPYInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<CPYInstruction> registrar("CPYInstruction");
}

void CPYInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, absolute};
    vector<uint_least8_t> opcodeList{              0xC0,     0xC4,     0xCC};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "CPY", CPYInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t CPYInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.Flags.Carry = cpu.Y >= value;
    return cpu.Y - value;
}
