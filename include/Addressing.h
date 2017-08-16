#ifndef ADDRESSING_H
#define ADDRESSING_H

#include "Utils.h"
using namespace std;

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
    //virtual uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData) = 0;
    virtual uint_least16_t printAddress(const uint_least16_t &address) = 0;
};

class ImplictAddressing : public Addressing {
public:
	static u8 length;
    ImplictAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AccumulatorAddressing : public Addressing  {
public:
	static u8 length;
    AccumulatorAddressing();
	static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ImmediateAddressing : public Addressing  {
public:
	static u8 length;
    ImmediateAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageAddressing : public Addressing  {
public:
	static u8 length;
    ZeroPageAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageXAddressing : public Addressing  {
public:
	static u8 length;
    ZeroPageXAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class ZeroPageYAddressing : public Addressing  {
public:
	static u8 length;
    ZeroPageYAddressing();
	static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteAddressing : public Addressing  {
public:
	static u8 length;
    AbsoluteAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteXAddressing : public Addressing  {
public:
	static u8 length;
    AbsoluteXAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class AbsoluteYAddressing : public Addressing  {
public:
	static u8 length;
    AbsoluteYAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class RelativeAddressing : public Addressing  {
public:
	static u8 length;
    RelativeAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectAddressing : public Addressing  {
public:
	static u8 length;
    IndirectAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectXAddressing : public Addressing  {
public:
	static u8 length;
    IndirectXAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

class IndirectYAddressing : public Addressing  {
public:
	static u8 length;
    IndirectYAddressing();
    static uint_least16_t getAddress(CPU &cpu, const uint_least16_t &instructionData);
	uint_least16_t printAddress(const uint_least16_t &address) override;
};

#endif