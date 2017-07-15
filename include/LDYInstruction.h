#ifndef LDY_INSTRUCTION_H
#define LDY_INSTRUCTION_H

#include "Instruction.h"
#include "Addressing.h"

class LDYInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    void execute(CPU& cpu, const uint_least16_t &value) override;
    static vector<shared_ptr<Instruction>> createInstructions();
};


#endif