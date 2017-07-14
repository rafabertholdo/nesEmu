#include "Instruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

Instruction::Instruction(const AddressingMode &addressingMode, const uint_least8_t &opcode, const uint_least8_t &length, const string &menmonic) {
    Instruction::addressingMode = addressingMode;
    Instruction::opcode = opcode;
    Instruction::length = length;
    Instruction::menmonic = menmonic;
}

std::unordered_map<std::string, Instruction::create_f *> & Instruction::registry() {
    static std::unordered_map<std::string, Instruction::create_f *> impl;
    return impl;
}

void Instruction::execute(CPU& cpu, const uint_least16_t &value) {    
    std::cout << menmonic << " ";         
}