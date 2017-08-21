#include "Addressing.h"
#include "CPU.h"
#include "Utils.cpp"

using namespace std;

bool cross(u16 a, u8 i) { 
    return ((a+i) & 0xFF00) != ((a & 0xFF00)); 
}

uint_least16_t printSpaces(const uint_least16_t &length) {
    cout << std::setw(length) << std::setfill(' ') << " ";
	return length;
}


uint_least16_t Addressing::printAddress(const uint_least16_t &address) {
	return Utils<uint_least16_t>::printHex(address);
}

//ImplictAddressing
ImplictAddressing::ImplictAddressing() {
    length = 1;
}

u8 ImplictAddressing::length = 1;

uint_least16_t ImplictAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    return 0;
}

uint_least16_t ImplictAddressing::printAddress(const uint_least16_t &address) {
    return printSpaces(28);
}

//AccumulatorAddressing
AccumulatorAddressing::AccumulatorAddressing() {
    length = 1;
}

u8 AccumulatorAddressing::length = 1;

uint_least16_t AccumulatorAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    return cpu.A;
};

uint_least16_t AccumulatorAddressing::printAddress(const uint_least16_t &address){
    cout << "A";
	return printSpaces(27);
}

//ImmediateAddressing
ImmediateAddressing::ImmediateAddressing() {
    length = 2;
}

u8 ImmediateAddressing::length = 2;

uint_least16_t ImmediateAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    return instructionData;
};

uint_least16_t ImmediateAddressing::printAddress(const uint_least16_t &address){
    cout << "#$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(26 - addressSize);
}

//ZeroPageAddressing
ZeroPageAddressing::ZeroPageAddressing() {
    length = 2;
}

u8 ZeroPageAddressing::length = 2;

uint_least16_t ZeroPageAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    return instructionData;
};

uint_least16_t ZeroPageAddressing::printAddress(const uint_least16_t &address) {
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//ZeroPageXAddressing
ZeroPageXAddressing::ZeroPageXAddressing() {
    length = 2;
}

u8 ZeroPageXAddressing::length = 2;

uint_least16_t ZeroPageXAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    cpu.tick();
    return ((instructionData + cpu.X) % 256);
};

uint_least16_t ZeroPageXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

//ZeroPageYAddressing
ZeroPageYAddressing::ZeroPageYAddressing() {
    length = 2;
}

u8 ZeroPageYAddressing::length = 2;

uint_least16_t ZeroPageYAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    cpu.tick();
    return ((instructionData + cpu.Y) % 256);
};

uint_least16_t ZeroPageYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

//AbsoluteAddressing
AbsoluteAddressing::AbsoluteAddressing() {
    length = 3;
}

u8 AbsoluteAddressing::length = 3;

uint_least16_t AbsoluteAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    return instructionData;
};

uint_least16_t AbsoluteAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//AbsoluteXAddressing
AbsoluteXAddressing::AbsoluteXAddressing() {
    length = 3;
}

u8 AbsoluteXAddressing::length = 3;

uint_least16_t AbsoluteXAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    if (cross(instructionData, cpu.X)) {
        cpu.tick();
    }
    return instructionData + cpu.X;
};

uint_least16_t AbsoluteXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

//AbsoluteYAddressing
AbsoluteYAddressing::AbsoluteYAddressing() {
    length = 3;
}

u8 AbsoluteYAddressing::length = 3;

uint_least16_t AbsoluteYAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    if (cross(instructionData, cpu.Y)) {
        cpu.tick();
    }
    return instructionData + cpu.Y;
};

uint_least16_t AbsoluteYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

//RelativeAddressing
RelativeAddressing::RelativeAddressing() {
    length = 2;
}

u8 RelativeAddressing::length = 2;

uint_least16_t RelativeAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    s8 address = instructionData;
    return cpu.PC + address;      
};

uint_least16_t RelativeAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//IndirectAddressing
IndirectAddressing::IndirectAddressing() {
    length = 3;
}

u8 IndirectAddressing::length = 3;

uint_least16_t IndirectAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    u8 lsb = cpu.read(instructionData);

    //fix page boundaries
    u16 msbAddress = instructionData + 1;
    if (msbAddress % 0x100 == 0) {
        msbAddress = instructionData - 0xFF;
    }
    u8 msb = cpu.read(msbAddress);    

    return lsb + (msb << 8); 
};

uint_least16_t IndirectAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
	auto addressSize = Addressing::printAddress(address);
    cout << ")";
	return printSpaces(25 - addressSize);
}

//IndirectXAddressing
IndirectXAddressing::IndirectXAddressing() {
    length = 2;
}

u8 IndirectXAddressing::length = 2;

uint_least16_t IndirectXAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    u8 lsb = cpu.read((instructionData + cpu.X) % 256);
    u8 msb = cpu.read((instructionData + cpu.X + 1) % 256);
    return lsb + (msb << 8); 
};

uint_least16_t IndirectXAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
	auto addressSize = Addressing::printAddress(address);
    cout << ",X)";
	return printSpaces(23 - addressSize);
}

//IndirectYAddressing
IndirectYAddressing::IndirectYAddressing() {
    length = 2;
}

u8 IndirectYAddressing::length = 2;

uint_least16_t IndirectYAddressing::getAddress(CPU &cpu, const uint_least16_t &instructionData) {
    u8 lsb = cpu.read(instructionData);
    u8 msb = cpu.read((instructionData + 1) % 256);
    u16 value = lsb + (msb << 8);
    if (cross(value - cpu.Y, cpu.Y)) {
        cpu.tick();
    }
    return value + cpu.Y;
};

uint_least16_t IndirectYAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y)";
	return printSpaces(23 - addressSize);
}