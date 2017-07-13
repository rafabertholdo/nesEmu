#ifndef SEI_INSTRUCTION_H
#define SEI_INSTRUCTION_H

#include "Instruction.h"

class SEIInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
    static vector<shared_ptr<Instruction>> createInstructions();
};

#endif