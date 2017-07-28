#ifndef CLV_INSTRUCTION_H
#define CLV_INSTRUCTION_H

#include "Instruction.h"

class CLVInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static void createInstructions(vector<unique_ptr<Instruction>> &instructions);         
    static void createInstructions2(vector<Instruction> &instructions);    
};


#endif