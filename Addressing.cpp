#include "Addressing.h"
#include "CPU.h"
#include "Utils.cpp"

uint_least16_t printSpaces(const uint_least16_t &length) {
    cout << std::setw(length) << std::setfill(' ') << " ";
	return length;
}

uint_least16_t Addressing::printAddress(const uint_least16_t &address){
    return Utils<uint_least16_t>::printHex(address);    
}


uint_least16_t ImplictAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return 0;
}

uint_least16_t ImplictAddressing::printAddress(const uint_least16_t &address){
    return printSpaces(28);
}

uint_least16_t AccumulatorAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return cpu.A;
};

uint_least16_t AccumulatorAddressing::printAddress(const uint_least16_t &address){
    cout << "A";
	return printSpaces(27);
}

uint_least16_t ImmediateAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

uint_least16_t ImmediateAddressing::printAddress(const uint_least16_t &address){
    cout << "#$";    
	return printSpaces(26 - Addressing::printAddress(address));
}

uint_least16_t ZeroPageAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

uint_least16_t ZeroPageAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
	return printSpaces(27 - Addressing::printAddress(address));
}

uint_least16_t ZeroPageXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return ((rawAdrress + cpu.X) % 256);
};

uint_least16_t ZeroPageXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

uint_least16_t ZeroPageYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return ((rawAdrress + cpu.Y) % 256);
};

uint_least16_t ZeroPageYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

uint_least16_t AbsoluteAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return Utils<u8>::getLittleEndianValue(instructionData);
};

uint_least16_t AbsoluteAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
	return printSpaces(27 - Addressing::printAddress(address));
}

uint_least16_t AbsoluteXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return rawAdrress + cpu.X;
};

uint_least16_t AbsoluteXAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",X";
	return printSpaces(25 - addressSize);
}

uint_least16_t AbsoluteYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return rawAdrress + cpu.Y;
};

uint_least16_t AbsoluteYAddressing::printAddress(const uint_least16_t &address){
    cout << "$";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y";
	return printSpaces(25 - addressSize);
}

uint_least16_t RelativeAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    s8 address = instructionData[0];
    return cpu.PC + address;      
};

uint_least16_t RelativeAddressing::printAddress(const uint_least16_t &address){
    cout << "$";    
	return printSpaces(27 - Addressing::printAddress(address));
}

uint_least16_t IndirectAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    u8 lsb = cpu.read(rawAdrress);
    u8 msb = cpu.read(rawAdrress + 1);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}); 
};

uint_least16_t IndirectAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
	auto addressSize = Addressing::printAddress(address);
    cout << ")";
	return printSpaces(25 - addressSize);
}

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
	return printSpaces(19);
}

uint_least16_t IndirectYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
    u8 lsb = cpu.read(rawAdrress);
    u8 msb = cpu.read((rawAdrress + 1) % 256);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}) + cpu.Y;
};

uint_least16_t IndirectYAddressing::printAddress(const uint_least16_t &address){
    cout << "($";
    auto addressSize = Addressing::printAddress(address);
    cout << ",Y)";
	return printSpaces(25 - addressSize);
}