#include "Instructions/EORInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<EORInstruction> registrar("EORInstruction");
}

void EORInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x49,     0x45,      0x55,     0x4D,      0x5D,      0x59,      0x41,      0x51};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "EOR", EORInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);
    }    
}

uint_least16_t EORInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    return cpu.A ^= value;
}
