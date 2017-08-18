#ifndef BVC_INSTRUCTION_H
#define BVC_INSTRUCTION_H

#include "Instruction.h"

/*
BVC - Branch if Overflow Clear

If the overflow flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
*/
class BVCInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);              
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);       
};


#endif