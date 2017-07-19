#ifndef ASL_INSTRUCTION_H
#define ASL_INSTRUCTION_H

#include "Instruction.h"

class ASLInstruction : public Instruction {    
public:    
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();    
};


#endif