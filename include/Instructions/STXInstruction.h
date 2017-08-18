#ifndef STX_INSTRUCTION_H
#define STX_INSTRUCTION_H

#include "Instruction.h"

/*
STX - Store X Register

M = X

Stores the contents of the X register into memory.
*/
class STXInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif