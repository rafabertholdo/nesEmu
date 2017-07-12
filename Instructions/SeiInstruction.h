#ifndef SEI_INSTRUCTION_H
#define SEI_INSTRUCTION_H

#include "Instruction.h"

class SEIInstruction : public Instruction {    
public:
    SEIInstruction();    
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
};

#endif