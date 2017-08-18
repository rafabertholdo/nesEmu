#ifndef LSR_INSTRUCTION_H
#define LSR_INSTRUCTION_H

#include "Instruction.h"

/*
LSR - Logical Shift Right

A,C,Z,N = A/2 or M,C,Z,N = M/2

Each of the bits in A or M is shift one place to the right. The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
*/
class LSRInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static uint_least16_t sharedActionA(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);         
           
};


#endif