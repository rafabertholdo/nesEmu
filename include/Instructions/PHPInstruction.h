#ifndef PHP_INSTRUCTION_H
#define PHP_INSTRUCTION_H

#include "Instruction.h"

/*
PHP - Push Processor Status

Pushes a copy of the status flags on to the stack.
*/
class PHPInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);               
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);      
};

#endif