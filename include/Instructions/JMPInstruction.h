#ifndef JMP_INSTRUCTION_H
#define JMP_INSTRUCTION_H

#include "Instruction.h"

/*
JMP - Jump

Sets the program counter to the address specified by the operand.
*/
class JMPInstruction : public Instruction {    
public:    
    using Instruction::Instruction;    
    static void createInstructions(InstructionArray &instructions);                 
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);    
};

#endif