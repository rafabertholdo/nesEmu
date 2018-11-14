#ifndef SEI_INSTRUCTION_H
#define SEI_INSTRUCTION_H

#include "Instruction.h"

/*
SEI - Set Interrupt Disable

I = 1

Set the interrupt disable flag to one.
*/
class SEIInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(InstructionArray &instructions);         
        
};

#endif