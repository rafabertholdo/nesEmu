#include "Instruction.h"
#include "../Utils.cpp"
#include <iostream>
#include <iomanip>

using namespace std;


std::map<AddressingMode, std::tuple<u8, std::function<uint_least16_t(CPU& cpu, const uint_least16_t &instructionData)>>> Instruction::createAddressingMap() {

	std::map<AddressingMode, std::tuple<u8, std::function<uint_least16_t(CPU& cpu, const uint_least16_t &instructionData)>>> map = {
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

const std::map<AddressingMode, std::tuple<u8, std::function<uint_least16_t(CPU& cpu, const uint_least16_t &instructionData)>>>Instruction::addressingModes  =  Instruction::createAddressingMap();

Instruction::Instruction() {

}

Instruction::Instruction(const AddressingMode &addressingMode,
                         const uint_least8_t &opcode,                          
                         const string &menmonic,
                         const AffectFlags &&affectedFlags,                         
                         const bool &readsFromMemory) {
	
	auto addressingWrapper = Instruction::addressingModes.at(addressingMode);
    setAddressingLambda(get<1>(addressingWrapper));
    _opcode = opcode;
    _length = get<0>(addressingWrapper);
    _menmonic = menmonic;
    _affectedFlags.raw = static_cast<uint_least8_t>(affectedFlags);
    setReadsFromMemory(readsFromMemory);
}

Instruction::Instruction(const AddressingMode &addressingMode, 
                         const uint_least8_t &opcode,                 
                         const uint_least8_t &length,                 
                         const string &menmonic,
                         const AffectFlags &&affectedFlags,
                         const bool &readsFromMemory) {
	auto addressingWrapper = Instruction::addressingModes.at(addressingMode);
	setAddressingLambda(get<1>(addressingWrapper));
    _opcode = opcode;
	_length = get<0>(addressingWrapper);
    _menmonic = menmonic;
    _affectedFlags.raw = static_cast<uint_least8_t>(affectedFlags);
    setReadsFromMemory(readsFromMemory);
}

Instruction::~Instruction() {
    
}

std::unordered_map<std::string, Instruction::create_f *> & Instruction::registry() {
    static std::unordered_map<std::string, Instruction::create_f *> impl;
    return impl;
}

std::unordered_map<std::string, Instruction::create_f2 *> & Instruction::registry2() {
    static std::unordered_map<std::string, Instruction::create_f2 *> impl;
    return impl;
}

void Instruction::registrate(std::string const & name, create_f * fp) {
    registry()[name] = fp;
}

void Instruction::registrate2(std::string const & name, create_f2 * fp) {
    registry2()[name] = fp;
}

/*
std::vector<std::shared_ptr<Instruction>> Instruction::instantiate(std::string const & name) {
    std::vector<std::shared_ptr<Instruction>> empty;
    auto it = registry().find(name);
    return it == registry().end() ? empty : (it->second)();
}
*/

void Instruction::instantiateAll(std::vector<std::unique_ptr<Instruction>> &instructions) {    
    for(auto it = registry().begin(); it != registry().end(); ++it) { 
        (it->second)(instructions);
    }    
}

void Instruction::instantiateAll(std::vector<Instruction> &instructions) {
     for(auto it = registry2().begin(); it != registry2().end(); ++it) { 
         (it->second)(instructions);
     }        
 }

uint_least8_t Instruction::getOpcode() const {
    return _opcode;
}

string Instruction::getMenmonic() const {
    return _menmonic;
}

bool Instruction::readsFromMemory() const {
    return _readsFromMemory;
}

void Instruction::setReadsFromMemory(bool readsFromMemory) {
    dynamic_cast<ImmediateAddressing*>(_addressing.get()) ? _readsFromMemory = false : _readsFromMemory = readsFromMemory;
}

void Instruction::setLambda(std::function<uint_least16_t(CPU&,const uint_least16_t &)> lambda) {
    _lambda = lambda;
}

void Instruction::setAddressingLambda(std::function<uint_least16_t(CPU&,const uint_least16_t &)> lambda) {
    _addressingLambda = lambda;
}

uint_least8_t Instruction::getLength() const {
    //return _addressing->length;
    return _length;
}

void Instruction::printAddress(CPU& cpu,  const uint_least16_t &instructionData) const {
    //u16 instructionValue = _addressing->getAddress(cpu, instructionData);
    //_addressing->printAddress(instructionValue);
}

uint_least16_t Instruction::action(CPU& cpu,  const uint_least16_t &instructionData) {
    return _lambda(cpu, instructionData);
}

void Instruction::execute(CPU& cpu,  const uint_least16_t &instructionData) {        
	u16 instructionValue = _addressingLambda(cpu, instructionData);
    
    if (_readsFromMemory) {
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

uint_least16_t ClearInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.tick();
    return 0;
}

uint_least16_t ClearInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return ClearInstruction::sharedAction(cpu, value);
}

uint_least16_t SetInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    return 1;
}

uint_least16_t SetInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return SetInstruction::sharedAction(cpu, value);
}

