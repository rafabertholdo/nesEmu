#ifndef ORA_INSTRUCTION_H
#define ORA_INSTRUCTION_H

#include "Instruction.h"

/*
ORA - Logical Inclusive OR

A,Z,N = A|M

An inclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
*/
class ORAInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static void createInstructions(InstructionArray &instructions);                 
};

#endif