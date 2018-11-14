#ifndef NMI_INSTRUCTION_H
#define NMI_INSTRUCTION_H

#include "Instruction.h"

/*
NMI: Non-Maskable Interrupt

The NMI instruction forces the generation of an interrupt request. 
The program counter and processor status are pushed on the stack then the NMI interrupt 
vector at $FFFA/B is loaded into the PC and the break flag in the status set to zero.
*/
class NMIInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif