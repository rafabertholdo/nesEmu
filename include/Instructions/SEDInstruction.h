#ifndef SED_INSTRUCTION_H
#define SED_INSTRUCTION_H

#include "Instruction.h"

class SEDInstruction : public SetInstruction {    
public:
    using SetInstruction::SetInstruction;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);    
};

#endif