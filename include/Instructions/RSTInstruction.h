#ifndef RST_INSTRUCTION_H
#define RST_INSTRUCTION_H

#include "Instruction.h"

/*
RST: Reset

The RST instruction forces the generation of an interrupt request. 
The stack then the RST interrupt vector at $FFFC/D is loaded into the PC.
*/
class RSTInstruction : public Instruction {    
public:
    using Instruction::Instruction;        
    static void createInstructions(InstructionArray &instructions);             
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif