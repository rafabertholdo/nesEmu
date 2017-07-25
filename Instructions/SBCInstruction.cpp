#include "Instructions/SBCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SBCInstruction> registrar("SBCInstruction");
}

vector<shared_ptr<Instruction>> SBCInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0xE9,      0xEB,     0xE5,      0xF5,     0xED,      0xFD,      0xF9,      0xE1,      0xF1};
    vector<uint_least8_t> lengthList{                 2,         2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<SBCInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "SBC", AffectFlags::Negative | AffectFlags::Zero);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
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

uint_least16_t SBCInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return SBCInstruction::sharedAction(cpu,value);
}