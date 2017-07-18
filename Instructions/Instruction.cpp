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
    Instruction::printsActionValue = false;
    Instruction::readsFromMemory = false;
}

std::unordered_map<std::string, Instruction::create_f *> & Instruction::registry() {
    static std::unordered_map<std::string, Instruction::create_f *> impl;
    return impl;
}

void Instruction::execute(CPU& cpu,  const vector<uint_least8_t> &instructionData) {    
    std::cout << menmonic << " ";
    CPU cpuCopy = cpu;   
    
    u16 instructionValue = applyAddressing(cpu, instructionData);              
    //auto charCount = addressing->printAddress(instructionValue, !printsActionValue);   
    auto charCount = addressing->printAddress(instructionValue, true);   
    
    if (readsFromMemory && !dynamic_cast<ImmediateAddressing*>(addressing.get())) {
        instructionValue = cpu.read(instructionValue);
    }
/*
    if (printsActionValue) {
        cout << " = ";
        auto valueFromMem = cpu.read(instructionValue);
        auto actionValueSize = Utils<uint_least8_t>::printHex(valueFromMem);
        cout << std::setw(28 - charCount - 3 - actionValueSize) << std::setfill(' ') << " ";        
    }
*/
    auto actionValue = action(cpu, instructionValue);    
    cpuCopy.dumpRegs();
    changeFlags(cpu, instructionValue, actionValue);
}

uint_least16_t Instruction::applyAddressing(CPU& cpu,  const vector<uint_least8_t> &instructionData) {
    u16 instructionValue = addressing->getAddress(cpu, instructionData);    
    return instructionValue;
}

void Instruction::changeFlags(CPU& cpu,  const uint_least16_t &value, const uint_least16_t &actionValue) {
    if (_affectedFlags.raw) {
        if (_affectedFlags.Zero) {
            updateZero(cpu, value, actionValue);
        }
        
        if (_affectedFlags.Negative) {
            updateNegative(cpu, value, actionValue);
        }

        if (_affectedFlags.Carry) {
            updateCarry(cpu, value, actionValue);
        }

        if (_affectedFlags.DecimalMode) {
            updateDecimalMode(cpu, value, actionValue);
        }

		if (_affectedFlags.Overflow) {
			updateOverflow(cpu, value, actionValue);
		}

		if (_affectedFlags.InterruptEnabled) {
			updateInterruptEnabled(cpu, value, actionValue);
		}
    }
}

void Instruction::updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Carry = actionValue > 0;
}

void Instruction::updateZero(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Zero = !actionValue;
}

void Instruction::updateInterruptEnabled(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
	cpu.Flags.InterruptEnabled = actionValue > 0;
}

void Instruction::updateDecimalMode(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.DecimalMode = actionValue > 0;
}

void Instruction::updateOverflow(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {	
	cpu.Flags.Overflow = actionValue > 0;
}

void Instruction::updateNegative(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    std::bitset<8> set(actionValue);
    cpu.Flags.Negative = set.test(7);
}

uint_least16_t ClearInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return 0;
}

uint_least16_t SetInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return 1;
}