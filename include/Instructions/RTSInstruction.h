#ifndef RTS_INSTRUCTION_H
#define RTS_INSTRUCTION_H

#include "Instruction.h"

/*
RTS - Return from Subroutine

The RTS instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack.
*/
class RTSInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);               
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif