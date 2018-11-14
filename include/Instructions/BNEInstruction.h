#ifndef BNE_INSTRUCTION_H
#define BNE_INSTRUCTION_H

#include "Instruction.h"

/*
BNE - Branch if Not Equal

If the zero flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BNEInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif