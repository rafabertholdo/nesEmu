#ifndef LDY_INSTRUCTION_H
#define LDY_INSTRUCTION_H

#include "Instruction.h"
#include "Addressing.h"

/*
LDY - Load Y Register

Y,Z,N = M

Loads a byte of memory into the Y register setting the zero and negative flags as appropriate.
*/
class LDYInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif