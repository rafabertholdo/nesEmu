#ifndef SEI_INSTRUCTION_H
#define SEI_INSTRUCTION_H

#include "Instruction.h"

class SEIInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);    
};

#endif