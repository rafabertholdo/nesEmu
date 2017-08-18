#ifndef CLC_INSTRUCTION_H
#define CLC_INSTRUCTION_H

#include "Instruction.h"

/*
CLC - Clear Carry Flag

C = 0

Set the carry flag to zero.
*/
class CLCInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(InstructionArray &instructions);            
};

#endif