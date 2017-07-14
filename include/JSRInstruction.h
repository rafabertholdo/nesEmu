#ifndef JSR_INSTRUCTION_H
#define JSR_INSTRUCTION_H

#include "Instruction.h"

//Jump to SubRoutine
class JSRInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    void execute(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif