#ifndef BCC_INSTRUCTION_H
#define BCC_INSTRUCTION_H

#include "Instruction.h"

/*
BCC - Branch if Carry Clear

If the carry flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BCCInstruction : public Instruction {    
public:    
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray&instructions);    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};


#endif