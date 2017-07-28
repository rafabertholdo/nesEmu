#ifndef SEC_INSTRUCTION_H
#define SEC_INSTRUCTION_H

#include "Instruction.h"

class SECInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);    
};

#endif