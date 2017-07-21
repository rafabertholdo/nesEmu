#include "Addressing.h"
#include "CPU.h"
#include "Utils.cpp"

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
uint_least16_t ImplictAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return 0;
}

uint_least16_t ImplictAddressing::printAddress(const uint_least16_t &address) {
    return printSpaces(28);
}

//AccumulatorAddressing
uint_least16_t AccumulatorAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return cpu.A;
};

uint_least16_t AccumulatorAddressing::printAddress(const uint_least16_t &address){
    cout << "A";
	return printSpaces(27);
}

//ImmediateAddressing
uint_least16_t ImmediateAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

uint_least16_t ImmediateAddressing::printAddress(const uint_least16_t &address){
    cout << "#$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(26 - addressSize);
}

//ZeroPageAddressing
uint_least16_t ZeroPageAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

uint_least16_t ZeroPageAddressing::printAddress(const uint_least16_t &address) {
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//ZeroPageXAddressing
uint_least16_t ZeroPageXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    cpu.tick();
    return ((rawAdrress + cpu.X) % 256);
};

uint_least16_t ZeroPageXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

//ZeroPageYAddressing
uint_least16_t ZeroPageYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    cpu.tick();
    return ((rawAdrress + cpu.Y) % 256);
};

uint_least16_t ZeroPageYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

//AbsoluteAddressing
uint_least16_t AbsoluteAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return Utils<u8>::getLittleEndianValue(instructionData);
};

uint_least16_t AbsoluteAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//AbsoluteXAddressing
uint_least16_t AbsoluteXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAddresss = Utils<u8>::getLittleEndianValue(instructionData);
    if (cross(rawAddresss, cpu.X)) {
        cpu.tick();
    }
    return rawAddresss + cpu.X;
};

uint_least16_t AbsoluteXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

//AbsoluteYAddressing
uint_least16_t AbsoluteYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAddresss = Utils<u8>::getLittleEndianValue(instructionData);
    if (cross(rawAddresss, cpu.Y)) {
        cpu.tick();
    }
    return rawAddresss + cpu.Y;
};

uint_least16_t AbsoluteYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

//RelativeAddressing
uint_least16_t RelativeAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    s8 address = instructionData[0];
    return cpu.PC + address;      
};

uint_least16_t RelativeAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
    auto addressSize = Addressing::printAddress(address);
	return printSpaces(27 - addressSize);
}

//IndirectAddressing
uint_least16_t IndirectAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 lsbAddress = Utils<u8>::getLittleEndianValue(instructionData);    
    u8 lsb = cpu.read(lsbAddress);

    //fix page boundaries
    u16 msbAddress = lsbAddress + 1;
    if (msbAddress % 0x100 == 0) {
        msbAddress = lsbAddress - 0xFF;
    }
    u8 msb = cpu.read(msbAddress);    

    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}); 
};

uint_least16_t IndirectAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
	auto addressSize = Addressing::printAddress(address);
    cout << ")";
	return printSpaces(25 - addressSize);
}

//IndirectXAddressing
uint_least16_t IndirectXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
    u8 lsb = cpu.read((rawAdrress + cpu.X) % 256);
    u8 msb = cpu.read((rawAdrress + cpu.X + 1) % 256);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}); 
};

uint_least16_t IndirectXAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
	auto addressSize = Addressing::printAddress(address);
    cout << ",X)";
	return printSpaces(23 - addressSize);
}

//IndirectYAddressing
uint_least16_t IndirectYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
    u8 lsb = cpu.read(rawAdrress);
    u8 msb = cpu.read((rawAdrress + 1) % 256);
    u16 value = Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb});
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