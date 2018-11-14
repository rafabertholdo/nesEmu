#ifndef CMP_INSTRUCTION_H
#define CMP_INSTRUCTION_H

#include "Instruction.h"

/*
CMP - Compare

Z,C,N = A-M

This instruction compares the contents of the accumulator with another memory held value and sets the zero and carry flags as appropriate.
*/
class CMPInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif