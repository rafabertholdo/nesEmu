#ifndef CLV_INSTRUCTION_H
#define CLV_INSTRUCTION_H

#include "Instruction.h"

class CLVInstruction : public ClearInstruction {    
public:    
    using ClearInstruction::ClearInstruction;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif