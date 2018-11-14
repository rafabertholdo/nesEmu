#include "Instructions/SBCInstruction.h"
#include "Addressing.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<SBCInstruction> registrar("SBCInstruction");
}

void SBCInstruction::createInstructions(InstructionArray &instructions) {    
    vector<AddressingMode> addressingModeList{immediate, immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xE9,      0xEB,     0xE5,      0xF5,     0xED,      0xFD,      0xF9,      0xE1,      0xF1};

    for(unsigned long i=0; i < opcodeList.size(); i++) {
        instructions[opcodeList[i]] = Instruction(addressingModeList[i], opcodeList[i], "SBC", SBCInstruction::sharedAction, AffectFlags::Negative | AffectFlags::Zero, true);        
    }    
}

uint_least16_t SBCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    uint_least8_t complement = (0xFF - value);
    uint_least16_t sum = cpu.A + complement + cpu.Flags.Carry;    
    //positive + positive = negative
    //negative + negative = positive
    cpu.Flags.Overflow = (cpu.A ^ sum) & (complement ^ sum) & 0x80;

    //The result of every ADC instruction is a nine-bit value in the range zero (0 + 0 + 0) to 511 (255 + 1 + 255). 
    //The least significant eight bits (zero to 255) are stored in the accumulator and the most significant bit in 
    //the carry flag.
    cpu.Flags.Carry = sum > 0xFF;
    return cpu.A = sum % 0x100;
}
