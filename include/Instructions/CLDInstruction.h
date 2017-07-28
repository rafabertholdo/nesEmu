#ifndef CLD_INSTRUCTION_H
#define CLD_INSTRUCTION_H

#include "Instruction.h"

class CLDInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);         
};


#endif