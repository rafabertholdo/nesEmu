#ifndef BRK_INSTRUCTION_H
#define BRK_INSTRUCTION_H

#include "Instruction.h"

/*
BRK - Force Interrupt

The BRK instruction forces the generation of an interrupt request. 
The program counter and processor status are pushed on the stack then the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break flag in the status set to one.
*/
class BRKInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif