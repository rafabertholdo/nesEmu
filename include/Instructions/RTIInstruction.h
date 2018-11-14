#ifndef RTI_INSTRUCTION_H
#define RTI_INSTRUCTION_H

#include "Instruction.h"

/*
RTI - Return from Interrupt

The RTI instruction is used at the end of an interrupt processing routine. It pulls the processor flags from the stack followed by the program counter.
*/
class RTIInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif