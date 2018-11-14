#ifndef SEC_INSTRUCTION_H
#define SEC_INSTRUCTION_H

#include "Instruction.h"

/*
SEC - Set Carry Flag

C = 1

Set the carry flag to one.
*/
class SECInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(InstructionArray &instructions);         
        
};

#endif