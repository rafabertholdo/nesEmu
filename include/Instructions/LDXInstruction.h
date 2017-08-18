#ifndef LDX_INSTRUCTION_H
#define LDX_INSTRUCTION_H

#include "Instruction.h"

/*
LDX - Load X Register

X,Z,N = M

Loads a byte of memory into the X register setting the zero and negative flags as appropriate.
*/
class LDXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif