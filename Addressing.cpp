#include "Addressing.h"
#include "CPU.h"
#include "Utils.cpp"

void Addressing::printAddress(const uint_least16_t &address){
    Utils<uint_least16_t>::printHex(address);
    cout << "\t";
}


uint_least16_t ImplictAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return 0;
}

void ImplictAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t AccumulatorAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return cpu.A;
};

void AccumulatorAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t ImmediateAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

void ImmediateAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t ZeroPageAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return instructionData[0];
};

void ZeroPageAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t ZeroPageXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return ((rawAdrress + cpu.X) % 256);
};

void ZeroPageXAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t ZeroPageYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return ((rawAdrress + cpu.Y) % 256);
};

void ZeroPageYAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t AbsoluteAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    return Utils<u8>::getLittleEndianValue(instructionData);
};

void AbsoluteAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t AbsoluteXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return rawAdrress + cpu.X;
};

void AbsoluteXAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t AbsoluteYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    return rawAdrress + cpu.Y;
};

void AbsoluteYAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t RelativeAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    s8 address = instructionData[0];
    return cpu.PC + address;      
};

void RelativeAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t IndirectAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);
    u8 lsb = cpu.read(rawAdrress);
    u8 msb = cpu.read(rawAdrress + 1);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}); 
};

void IndirectAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t IndirectXAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
    u8 lsb = cpu.read((rawAdrress + cpu.X) % 256);
    u8 msb = cpu.read((rawAdrress + cpu.X + 1) % 256);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}); 
};

void IndirectXAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}

uint_least16_t IndirectYAddressing::getAddress(CPU &cpu, const std::vector<uint_least8_t> &instructionData) {
    u16 rawAdrress = Utils<u8>::getLittleEndianValue(instructionData);                
    u8 lsb = cpu.read(rawAdrress);
    u8 msb = cpu.read((rawAdrress + 1) % 256);
    return Utils<u8>::getLittleEndianValue(vector<u8>{lsb,msb}) + cpu.Y;
};

void IndirectYAddressing::printAddress(const uint_least16_t &address){
    Addressing::printAddress(address);
}