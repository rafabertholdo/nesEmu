#include "Instructions/ADCInstruction.h"
#include "Addressing.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<ADCInstruction> registrar("ADCInstruction");
    Instruction::Registrar2<ADCInstruction> registrar2("ADCInstruction");
}

vector<shared_ptr<Instruction>> ADCInstruction::createInstructions() {
    vector<shared_ptr<Instruction>> instructions;

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x69,     0x65,      0x75,     0x6D,      0x7D,      0x79,      0x61,      0x71};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        auto instruction = make_shared<ADCInstruction>(addressingModeList[i], opcodeList[i], lengthList[i], "ADC", AffectFlags::Negative | AffectFlags::Zero);
        instruction->setReadsFromMemory(true);
        instructions.push_back(instruction);
    }
    return instructions;
}

void ADCInstruction::createInstructions2(vector<Instruction> &instructions) {    

    vector<AddressingMode> addressingModeList{immediate, zeroPage, zeroPageX, absolute, absoluteX, absoluteY, indirectX, indirectY};
    vector<uint_least8_t> opcodeList{              0x69,     0x65,      0x75,     0x6D,      0x7D,      0x79,      0x61,      0x71};
    vector<uint_least8_t> lengthList{                 2,        2,         2,        3,         3,         3,         2,         2};

    for(int i=0; i < opcodeList.size(); i++) {
        Instruction instruction(addressingModeList[i], opcodeList[i], lengthList[i], "ADC", AffectFlags::Negative | AffectFlags::Zero);
        instruction.setReadsFromMemory(true);  
        instruction.setLambda(ADCInstruction::sharedAction);
        instructions.at(instruction.getOpcode()) = instruction;        
    }    
}


uint_least16_t ADCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    auto sum = cpu.A + value + cpu.Flags.Carry;    
    //positive + positive = negative
    //negative + negative = positive
    cpu.Flags.Overflow = (cpu.A ^ sum) & (value ^ sum) & 0x80;

    //The result of every ADC instruction is a nine-bit value in the range zero (0 + 0 + 0) to 511 (255 + 1 + 255). 
    //The least significant eight bits (zero to 255) are stored in the accumulator and the most significant bit in 
    //the carry flag.
    cpu.Flags.Carry = sum > 0xFF;
    return cpu.A = sum % 0x100;
}

uint_least16_t ADCInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return ADCInstruction::sharedAction(cpu, value);
}