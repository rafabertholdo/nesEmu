#ifndef CPX_INSTRUCTION_H
#define CPX_INSTRUCTION_H

#include "Instruction.h"

/*
CPX - Compare X Register

Z,C,N = X-M

This instruction compares the contents of the X register with another memory held value and sets the zero and carry flags as appropriate.
*/
class CPXInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif