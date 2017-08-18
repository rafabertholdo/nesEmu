#ifndef ASL_INSTRUCTION_H
#define ASL_INSTRUCTION_H

#include "Instruction.h"

/*
ASL - Arithmetic Shift Left

A,Z,C,N = M*2 or M,Z,C,N = M*2

This operation shifts all the bits of the accumulator or memory contents one bit left. 
Bit 0 is set to 0 and bit 7 is placed in the carry flag. 
The effect of this operation is to multiply the memory contents by 2 (ignoring 2's complement considerations), setting the carry if the result will not fit in 8 bits.
*/
class ASLInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);    
    static void createInstructions(InstructionArray &instructions);    
};


#endif