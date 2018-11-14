#ifndef SRE_INSTRUCTION_H
#define SRE_INSTRUCTION_H

#include "Instruction.h"

/*
SRE - LSR + EOR

Shift right one bit in memory, then EOR accumulator with memory
*/
class SREInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);               
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif