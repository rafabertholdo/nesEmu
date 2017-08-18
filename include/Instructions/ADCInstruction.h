#ifndef ADC_INSTRUCTION_H
#define ADC_INSTRUCTION_H

#include "Instruction.h"

/*
ADC - Add with Carry

A,Z,C,N = A+M+C

This instruction adds the contents of a memory location to the accumulator together with the carry bit. 
If overflow occurs the carry bit is set, this enables multiple byte addition to be performed.
*/
class ADCInstruction : public Instruction {    
public:
    using Instruction::Instruction;    
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);        
    static void createInstructions(InstructionArray &instructions);    
};

#endif