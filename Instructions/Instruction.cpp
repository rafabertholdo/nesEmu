#include "Instruction.h"
#include "CPU.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;

map<AddressingMode, tuple<u8, getAddressFunctionPointer_t>> Instruction::createAddressingMap() {

	map<AddressingMode, tuple<u8, getAddressFunctionPointer_t>> map = {
	{implict     , { ImplictAddressing::length     , ImplictAddressing::getAddress     } },
	{accumulator , { AccumulatorAddressing::length , AccumulatorAddressing::getAddress } },
    {immediate   , { ImmediateAddressing::length   , ImmediateAddressing::getAddress   } },
    {zeroPage    , { ZeroPageAddressing::length    , ZeroPageAddressing::getAddress    } },
    {zeroPageX   , { ZeroPageXAddressing::length   , ZeroPageXAddressing::getAddress   } },
    {zeroPageY   , { ZeroPageYAddressing::length   , ZeroPageYAddressing::getAddress   } },
    {absolute    , { AbsoluteAddressing::length    , AbsoluteAddressing::getAddress    } },
    {absoluteX   , { AbsoluteXAddressing::length   , AbsoluteXAddressing::getAddress   } },
	{absoluteY   , { AbsoluteYAddressing::length   , AbsoluteYAddressing::getAddress   } },
    {relative    , { RelativeAddressing::length    , RelativeAddressing::getAddress    } },
    {indirect    , { IndirectAddressing::length    , IndirectAddressing::getAddress    } },
    {indirectX   , { IndirectXAddressing::length   , IndirectXAddressing::getAddress   } },
    {indirectY   , { IndirectYAddressing::length   , IndirectYAddressing::getAddress   } }
    };
	return map;
}

const map<AddressingMode, tuple<u8, getAddressFunctionPointer_t>>Instruction::addressingModes  =  Instruction::createAddressingMap();

Instruction::Instruction() {
	m_getAddressFunctionPointer = 0;

}

Instruction::Instruction(const AddressingMode &addressingMode,
                         const u8 &opcode,                          
                         const string &menmonic,
                         const actionFunctionPointer_t &actionFunctionPointer,
                         const AffectFlags &&affectedFlags,                         
                         const bool &readsFromMemory) {
	
	auto addressingWrapper = Instruction::addressingModes.at(addressingMode);
    m_getAddressFunctionPointer = get<1>(addressingWrapper);    
    m_actionFunctionPointer = actionFunctionPointer;
    m_opcode = opcode;
    m_length = get<0>(addressingWrapper);
    m_menmonic = menmonic;
    m_affectedFlags.raw = static_cast<u8>(affectedFlags);
    Instruction::readsFromMemory(readsFromMemory, addressingMode);
}

Instruction::~Instruction() {	
	m_getAddressFunctionPointer = 0;
	m_actionFunctionPointer = 0;
	m_opcode = 0;
	m_length = 0;
	m_menmonic = "";
	m_affectedFlags.raw = 0;
}

unordered_map<string, Instruction::create_f *> & Instruction::registry() {
    static unordered_map<string, Instruction::create_f *> impl;
    return impl;
}

void Instruction::registrate(const string &name, create_f *functionPointer) {
    registry()[name] = functionPointer;
}

void Instruction::instantiateAll(InstructionArray &instructions) {
     for(auto it = registry().begin(); it != registry().end(); ++it) { 
         (it->second)(instructions);
     }        
 }

const u8& Instruction::opcode() const {
    return m_opcode;
}

const string& Instruction::menmonic() const {
    return m_menmonic;
}

const bool& Instruction::readsFromMemory() const {
    return m_readsFromMemory;
}

void Instruction::readsFromMemory(const bool &readsFromMemory, const AddressingMode &addressingMode) {
    m_readsFromMemory = addressingMode == immediate ? false : readsFromMemory;
}

const u8& Instruction::length() const {    
    return m_length;
}

u16 Instruction::action(CPU& cpu,  const u16 &instructionData) {
    return m_actionFunctionPointer(cpu, instructionData);
}

void Instruction::execute(CPU& cpu,  const u16 &instructionData) {            
    u16 instructionValue = m_getAddressFunctionPointer(cpu, instructionData);    
    
    if (m_readsFromMemory) {
        instructionValue = cpu.read(instructionValue);
    }
    auto actionValue = action(cpu, instructionValue);
    changeFlags(cpu, instructionValue, actionValue);
}

void Instruction::changeFlags(CPU& cpu,  const u16 &value, const u16 &actionValue) {
    if (m_affectedFlags.raw) {
        if (m_affectedFlags.Zero) {
            updateZero(cpu, value, actionValue);
        }
        
        if (m_affectedFlags.Negative) {
            updateNegative(cpu, value, actionValue);
        }

        if (m_affectedFlags.Carry) {
            updateCarry(cpu, value, actionValue);
        }

        if (m_affectedFlags.DecimalMode) {
            updateDecimalMode(cpu, value, actionValue);
        }

		if (m_affectedFlags.Overflow) {
			updateOverflow(cpu, value, actionValue);
		}

		if (m_affectedFlags.InterruptDisabled) {
			updateInterruptDisabled(cpu, value, actionValue);
		}
    }
}

void Instruction::updateCarry(CPU& cpu, const u16 &value, const u16 &actionValue) {
    cpu.Flags.Carry = actionValue > 0;
}

void Instruction::updateZero(CPU& cpu, const u16 &value, const u16 &actionValue) {
    cpu.Flags.Zero = !actionValue;
}

void Instruction::updateInterruptDisabled(CPU& cpu, const u16 &value, const u16 &actionValue) {
	cpu.Flags.InterruptDisabled = actionValue > 0;
}

void Instruction::updateDecimalMode(CPU& cpu, const u16 &value, const u16 &actionValue) {
    cpu.Flags.DecimalMode = actionValue > 0;
}

void Instruction::updateOverflow(CPU& cpu, const u16 &value, const u16 &actionValue) {	
	cpu.Flags.Overflow = actionValue > 0;
}

void Instruction::updateNegative(CPU& cpu, const u16 &value, const u16 &actionValue) {
    bitset<8> set(actionValue);
    cpu.Flags.Negative = set.test(7);
}

u16 ClearInstruction::sharedAction(CPU& cpu, const u16 &value) {    
    cpu.tick();
    return 0;
}

u16 SetInstruction::sharedAction(CPU& cpu, const u16 &value) {
    cpu.tick();
    return 1;
}
