#ifndef BIT_INSTRUCTION_H
#define BIT_INSTRUCTION_H

#include "Instruction.h"

/*
BIT - Bit Test

A & M, N = M7, V = M6

This instructions is used to test if one or more bits are set in a target memory location. 
The mask pattern in A is ANDed with the value in memory to set or clear the zero flag, but the result is not kept. 
Bits 7 and 6 of the value from memory are copied into the N and V flags.
*/
class BITInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif