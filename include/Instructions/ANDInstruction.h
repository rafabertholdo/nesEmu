#ifndef AND_INSTRUCTION_H
#define AND_INSTRUCTION_H

#include "Instruction.h"

/*
AND - Logical AND

A,Z,N = A&M

A logical AND is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
*/
class ANDInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
    static void createInstructions(InstructionArray &instructions);
};


#endif