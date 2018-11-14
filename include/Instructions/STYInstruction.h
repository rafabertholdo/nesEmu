#ifndef STY_INSTRUCTION_H
#define STY_INSTRUCTION_H

#include "Instruction.h"

/*
STY - Store Y Register

M = Y

Stores the contents of the Y register into memory.
*/
class STYInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif