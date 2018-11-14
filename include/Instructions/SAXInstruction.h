#ifndef SAX_INSTRUCTION_H
#define SAX_INSTRUCTION_H

#include "Instruction.h"
/*
SAX - AND X register with accumulator and store result in memory
*/
class SAXInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);               
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif