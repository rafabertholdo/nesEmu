#ifndef CPY_INSTRUCTION_H
#define CPY_INSTRUCTION_H

#include "Instruction.h"

/*
CPY - Compare Y Register

Z,C,N = Y-M

This instruction compares the contents of the Y register with another memory held value and sets the zero and carry flags as appropriate.
*/
class CPYInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif