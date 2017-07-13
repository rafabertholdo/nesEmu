#ifndef ADDRESSING_H
#define ADDRESSING_H

#include "Utils.h"

enum AddressingMode {     
    immediate,
    absolute, 
    absoluteX,
    absoluteY,
    zeroPage,
    zeroPageX,
    zeroPageY,
    relative,
    implict,
    indirect,
    indirectX,
    indirectY,
    accumulator
};

class CPU;
class Addressing {
public:
    virtual uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) = 0;
};

class ImplictAddressing : public Addressing {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class AccumulatorAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class ImmediateAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class ZeroPageAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class ZeroPageXAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class ZeroPageYAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class AbsoluteAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class AbsoluteXAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class AbsoluteYAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class RelativeAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class IndirectAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class IndirectXAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

class IndirectYAddressing : public Addressing  {
public:
    uint_least16_t getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) override;
};

#endif