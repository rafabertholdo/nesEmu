#include "Instruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

std::map<AddressingMode, shared_ptr<Addressing>> Instruction::createAddressingMap() {
    std::map<AddressingMode, shared_ptr<Addressing>> map = {
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
    return map;
}

const std::map<AddressingMode, shared_ptr<Addressing>>Instruction::addressingModes  =  Instruction::createAddressingMap();

Instruction::Instruction(const AddressingMode &addressingMode,
                         const uint_least8_t &opcode, 
                         const uint_least8_t &length, 
                         const string &menmonic,
                         const AffectFlags &&affectedFlags) {
    Instruction::addressing = Instruction::addressingModes.at(addressingMode);
    Instruction::opcode = opcode;
    Instruction::length = length;
    Instruction::menmonic = menmonic;
    Instruction::_affectedFlags.raw = static_cast<uint_least8_t>(affectedFlags);
    Instruction::readsFromMemory = false;
}

std::unordered_map<std::string, Instruction::create_f *> & Instruction::registry() {
    static std::unordered_map<std::string, Instruction::create_f *> impl;
    return impl;
}

void Instruction::printAddress(CPU& cpu,  const uint_least16_t &instructionData) {
    u16 instructionValue = addressing->getAddress(cpu, instructionData);
    addressing->printAddress(instructionValue);
}

void Instruction::execute(CPU& cpu,  const uint_least16_t &instructionData) {        
    u16 instructionValue = addressing->getAddress(cpu, instructionData);    
    
    if (readsFromMemory && !dynamic_cast<ImmediateAddressing*>(addressing.get())) {
        instructionValue = cpu.read(instructionValue);
    }
    auto actionValue = action(cpu, instructionValue);
    changeFlags(cpu, instructionValue, actionValue);
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

		if (_affectedFlags.InterruptDisabled) {
			updateInterruptDisabled(cpu, value, actionValue);
		}
    }
}

void Instruction::updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Carry = actionValue > 0;
}

void Instruction::updateZero(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
    cpu.Flags.Zero = !actionValue;
}

void Instruction::updateInterruptDisabled(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue) {
	cpu.Flags.InterruptDisabled = actionValue > 0;
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
    //cpu.tick();
    return 0;
}

uint_least16_t SetInstruction::action(CPU& cpu, const uint_least16_t &value) {
    //cpu.tick();
    return 1;
}