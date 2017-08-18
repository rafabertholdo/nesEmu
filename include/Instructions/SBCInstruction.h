#ifndef SBC_INSTRUCTION_H
#define SBC_INSTRUCTION_H

#include "Instruction.h"

/*
SBC - Subtract with Carry

A,Z,C,N = A-M-(1-C)

This instruction subtracts the contents of a memory location to the accumulator together with the not of the carry bit. 
If overflow occurs the carry bit is clear, this enables multiple byte subtraction to be performed.
*/
class SBCInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif