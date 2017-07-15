#include "Instruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

std::map<AddressingMode, shared_ptr<Addressing>>Instruction::addressingModes = {
    {implict, make_shared<ImplictAddressing>()},
    {accumulator, make_shared<AccumulatorAddressing>()},                                
    {immediate, make_shared<ImmediateAddressing>()},
    {zeroPage, make_shared<ZeroPageAddressing>()},
    {zeroPageX, make_shared<ZeroPageXAddressing>()},
    {zeroPageY, make_shared<ZeroPageYAddressing>()},
    {absolute, make_shared<AbsoluteAddressing>()},
    {absoluteX, make_shared<AbsoluteXAddressing>()},
    {absoluteY, make_shared<AbsoluteYAddressing>()},
    {relative, make_shared<RelativeAddressing>()},
    {indirect, make_shared<IndirectAddressing>()},
    {indirectX, make_shared<IndirectXAddressing>()},
    {indirectY, make_shared<IndirectYAddressing>()}
};

Instruction::Instruction(const AddressingMode &addressingMode,
                         const uint_least8_t &opcode, 
                         const uint_least8_t &length, 
                         const string &menmonic,
                         const AffectFlags &&affectedFlags) {
    Instruction::addressing = Instruction::addressingModes[addressingMode];
    Instruction::opcode = opcode;
    Instruction::length = length;
    Instruction::menmonic = menmonic;
    Instruction::_affectedFlags.raw = static_cast<uint_least8_t>(affectedFlags);
}

std::unordered_map<std::string, Instruction::create_f *> & Instruction::registry() {
    static std::unordered_map<std::string, Instruction::create_f *> impl;
    return impl;
}

void Instruction::execute(CPU& cpu,  const vector<uint_least8_t> &instructionData) {    
    std::cout << menmonic << " ";
    u16 instructionValue = applyAddressing(cpu, instructionData);        
    changeFlags(cpu, action(cpu, instructionValue));
}

uint_least16_t Instruction::applyAddressing(CPU& cpu,  const vector<uint_least8_t> &instructionData) {
    u16 instructionValue = addressing->getAddress(cpu, instructionData);
    addressing->printAddress(instructionValue);
    return instructionValue;
}

void Instruction::changeFlags(CPU& cpu,  const uint_least16_t &value) {
    if (_affectedFlags.raw) {
        if (_affectedFlags.Zero) {
            cpu.Flags.Zero = value == 0;
        }
        
        if (_affectedFlags.Negative) {
            std::bitset<8> set(value);
            cpu.Flags.Negative = set.test(7);
        }

        if (_affectedFlags.Carry) {
            cpu.Flags.Carry = value;
        }

        if (_affectedFlags.DecimalMode) {
            cpu.Flags.DecimalMode = value;
        }
    }
}

uint_least16_t ClearInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return 0;
}

uint_least16_t SetInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return 1;
}