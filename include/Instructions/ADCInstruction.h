#ifndef ADC_INSTRUCTION_H
#define ADC_INSTRUCTION_H

#include "Instruction.h"

class ADCInstruction : public Instruction {    
public:
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value) override;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    static vector<shared_ptr<Instruction>> createInstructions();
    static void createInstructions2(vector<Instruction> &instructions);
    static void createInstructions3(vector<unique_ptr<Instruction>> &instructions);
};

#endif