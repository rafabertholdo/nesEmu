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

Instruction::Instruction() {

}

Instruction::Instruction(const AddressingMode &addressingMode,
                         const uint_least8_t &opcode, 
                         const uint_least8_t &length, 
                         const string &menmonic,
                         const AffectFlags &&affectedFlags) {
    _addressing = Instruction::addressingModes.at(addressingMode);
    _opcode = opcode;
    _length = _addressing->length;
    if (_addressing->length != length) {
        int a = 0;
    }
    _menmonic = menmonic;
    _affectedFlags.raw = static_cast<uint_least8_t>(affectedFlags);
    _readsFromMemory = false;
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

 std::vector<std::shared_ptr<Instruction>> Instruction::instantiate(std::string const & name) {
    std::vector<std::shared_ptr<Instruction>> empty;
    auto it = registry().find(name);
    return it == registry().end() ? empty : (it->second)();
}

std::vector<std::shared_ptr<Instruction>> Instruction::instantiateAll() {
    std::vector<std::shared_ptr<Instruction>> result;
    for(auto it = registry().begin(); it != registry().end(); ++it) { 
        for(auto specializedInstruction : (it->second)()) {
            result.push_back(specializedInstruction);
        }
        
    }
    return result;
}

void Instruction::instantiateAll(std::vector<Instruction> &instructions) {
     std::vector<Instruction>::iterator instrucitonsIterator;
     instrucitonsIterator = instructions.begin();

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
    _readsFromMemory = readsFromMemory;
}

void Instruction::setLambda(std::function<uint_least16_t(CPU&,const uint_least16_t &)> lambda) {
    _lambda = lambda;
}

uint_least8_t Instruction::getLength() const {
    //return _addressing->length;
    return _length;
}

void Instruction::printAddress(CPU& cpu,  const uint_least16_t &instructionData) const {
    u16 instructionValue = _addressing->getAddress(cpu, instructionData);
    _addressing->printAddress(instructionValue);
}

uint_least16_t Instruction::action(CPU& cpu,  const uint_least16_t &instructionData) {
    return _lambda(cpu, instructionData);
}

void Instruction::execute(CPU& cpu,  const uint_least16_t &instructionData) {        
    u16 instructionValue = _addressing->getAddress(cpu, instructionData);    
    
    if (_readsFromMemory && !dynamic_cast<ImmediateAddressing*>(_addressing.get())) {
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
    cpu.tick();
    return 0;
}

uint_least16_t SetInstruction::action(CPU& cpu, const uint_least16_t &value) {
    cpu.tick();
    return 1;
}

