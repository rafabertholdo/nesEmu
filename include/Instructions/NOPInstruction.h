#ifndef NOP_INSTRUCTION_H
#define NOP_INSTRUCTION_H

#include "Instruction.h"

/*
NOP - No Operation

The NOP instruction causes no changes to the processor other than the normal incrementing of the program counter to the next instruction.
*/
class NOPInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif