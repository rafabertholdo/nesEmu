#ifndef JSR_INSTRUCTION_H
#define JSR_INSTRUCTION_H

#include "Instruction.h"

/*
JSR - Jump to Subroutine

The JSR instruction pushes the address (minus one) of the return point on to the stack and then sets the program counter to the target memory address.
*/
class JSRInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif