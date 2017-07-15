#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "CPU.h"
#include "Addressing.h"
#include "RegBit.h"
#include <unordered_map>

class CPU; //forward declaration

enum class AffectFlags : uint_least8_t {
    None = 0,
    Carry = 1,
    Zero = 2,
    InterruptEnabled = 4,
    DecimalMode = 8,
    Overflow = 64,
    Negative = 128
};

inline AffectFlags operator|(AffectFlags a, AffectFlags b) {
    return static_cast<AffectFlags>(static_cast<uint_least8_t>(a) | static_cast<uint_least8_t>(b));
}

class Instruction {    
    using create_f = std::vector<std::shared_ptr<Instruction>>();    
    shared_ptr<Addressing> addressing;
    string menmonic;

    union 
    {
        uint_least8_t raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptEnabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        // 4,5 (0x10,0x20) don't exist
        RegBit<6> Overvlow; // overflow
        RegBit<7> Negative; // negative
    } _affectedFlags;

    static std::unordered_map<std::string, create_f *> & registry();    
    static std::map<AddressingMode, shared_ptr<Addressing>> addressingModes;
    uint_least16_t applyAddressing(CPU& cpu,  const vector<uint_least8_t> &instructionData);
    void changeFlags(CPU& cpu,  const uint_least16_t &value);

public:
    uint_least8_t length;
    uint_least8_t opcode;

    Instruction(const AddressingMode &addressingMode, 
                const uint_least8_t &opcode, 
                const uint_least8_t &length, 
                const string &menmonic,
                const AffectFlags &&affectedFlags = AffectFlags::None);
    
    void execute(CPU& cpu,  const vector<uint_least8_t> &instructionData);
    virtual uint_least16_t action(CPU& cpu,  const uint_least16_t &value) = 0;

    //dynamic instantiation
    virtual ~Instruction() = default;    

    static void registrate(std::string const & name, create_f * fp) {
        registry()[name] = fp;
    }

    static std::vector<std::shared_ptr<Instruction>> instantiate(std::string const & name) {
        std::vector<std::shared_ptr<Instruction>> empty;
        auto it = registry().find(name);
        return it == registry().end() ? empty : (it->second)();
    }

    static std::vector<std::shared_ptr<Instruction>> instantiateAll() {
        std::vector<std::shared_ptr<Instruction>> result;

        for(auto it = registry().begin(); it != registry().end(); ++it) { 
            for(auto specializedInstruction : (it->second)()) {
                result.push_back(specializedInstruction);
            }
            
        }
        return result;
    }

    template <typename D>
    struct Registrar
    {
        explicit Registrar(std::string const & name)
        {
            Instruction::registrate(name, &D::createInstructions);
        }        
    };
};

class ClearInstruction : public Instruction {
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value);
};

class SetInstruction : public Instruction {
    using Instruction::Instruction;
    uint_least16_t action(CPU& cpu, const uint_least16_t &value);
};

#endif