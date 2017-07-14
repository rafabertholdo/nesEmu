#ifndef STX_INSTRUCTION_H
#define STX_INSTRUCTION_H

#include "Instruction.h"

class STXInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    void execute(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif