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
    u8 length;
    virtual uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) = 0;
    virtual uint_least16_t printAddress(const uint_least16_t &address) = 0;
};

class ImplictAddressing : public Addressing {
public:
    ImplictAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AccumulatorAddressing : public Addressing  {
public:
    AccumulatorAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ImmediateAddressing : public Addressing  {
public:
    ImmediateAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageAddressing : public Addressing  {
public:
    ZeroPageAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageXAddressing : public Addressing  {
public:
    ZeroPageXAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageYAddressing : public Addressing  {
public:
    ZeroPageYAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteAddressing : public Addressing  {
public:
    AbsoluteAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteXAddressing : public Addressing  {
public:
    AbsoluteXAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteYAddressing : public Addressing  {
public:
    AbsoluteYAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class RelativeAddressing : public Addressing  {
public:
    RelativeAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectAddressing : public Addressing  {
public:
    IndirectAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectXAddressing : public Addressing  {
public:
    IndirectXAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectYAddressing : public Addressing  {
public:
    IndirectYAddressing();
    uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) override;
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

#endif