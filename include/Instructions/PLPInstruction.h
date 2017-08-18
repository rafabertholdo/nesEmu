#ifndef PLP_INSTRUCTION_H
#define PLP_INSTRUCTION_H

#include "Instruction.h"

/*
PLP - Pull Processor Status

Pulls an 8 bit value from the stack and into the processor flags. The flags will take on new states as determined by the value pulled.
*/
class PLPInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);              
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif