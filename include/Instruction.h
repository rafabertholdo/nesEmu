#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "CPU.h"
#include "Addressing.h"

class CPU; //forward declaration

class Instruction {    
public:
    uint_least8_t length;
    uint_least8_t opcode;
    AddressingMode addressingMode;
    string menmonic;

    Instruction(const AddressingMode &addressingMode, const uint_least8_t &opcode, const uint_least8_t &length, const string &menmonic);
    virtual ~Instruction();
    
    virtual void execute(CPU& cpu,  const uint_least16_t &value) = 0;
};

#endif