#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../CPU.h"
class CPU; //forward declaration

class Instruction {    
public:
    uint_least8_t length;
    uint_least8_t opcode;
    string menmonic;
    Instruction();
    virtual ~Instruction();
    virtual void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) = 0;
};

class SEIInstruction : public Instruction {    
public:
    SEIInstruction();    
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
};

class LDAInstruction : public Instruction {    
public:
    LDAInstruction();    
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
};

class CLDInstruction : public Instruction {    
public:
    CLDInstruction();    
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
};

class STAInstruction : public Instruction {    
public:
    STAInstruction();    
    void execute(CPU& cpu, const vector<uint_least8_t> &instructionData) override;
};
#endif