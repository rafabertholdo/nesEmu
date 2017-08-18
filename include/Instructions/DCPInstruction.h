#ifndef DCP_INSTRUCTION_H
#define DCP_INSTRUCTION_H

#include "Instruction.h"

/*
DCP - Decrement and Compare
This opcode DECs the contents of a memory location and then CMPs the result with the A register.
*/
class DCPInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif