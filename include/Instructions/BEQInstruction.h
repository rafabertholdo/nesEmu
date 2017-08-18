#ifndef BEQ_INSTRUCTION_H
#define BEQ_INSTRUCTION_H

#include "Instruction.h"

/*
BEQ - Branch if Equal

If the zero flag is set then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BEQInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif