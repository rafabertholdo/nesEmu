#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "CPU.h"
#include "Addressing.h"
#include "RegBit.h"
#include <unordered_map>
#include <functional>

class CPU; //forward declaration

enum class AffectFlags : uint_least8_t {
    None = 0,
    Carry = 1,
    Zero = 2,
    InterruptDisabled = 4,
    DecimalMode = 8,
    Overflow = 64,
    Negative = 128
};

inline AffectFlags operator|(AffectFlags a, AffectFlags b) {
    return static_cast<AffectFlags>(static_cast<uint_least8_t>(a) | static_cast<uint_least8_t>(b));
}

class Instruction {    
    using create_f = void(std::vector<unique_ptr<Instruction>> &instrucitons);
    using create_f2 = void(std::vector<Instruction> &instrucitons);
    
    union 
    {
        uint_least8_t raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptDisabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        // 4,5 (0x10,0x20) don't exist
        RegBit<6> Overflow; // overflow
        RegBit<7> Negative; // negative
    } _affectedFlags;

    static std::unordered_map<std::string, create_f *> & registry();    
    static std::unordered_map<std::string, create_f2 *> & registry2();        
    static const std::map<AddressingMode, shared_ptr<Addressing>> addressingModes;    
    static std::map<AddressingMode, shared_ptr<Addressing>> createAddressingMap();
    void changeFlags(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
protected:
    uint_least8_t _opcode;
    string _menmonic;
    bool _readsFromMemory;    
    shared_ptr<Addressing> _addressing;    
    uint_least8_t _length;
    std::function<uint_least16_t(CPU& cpu,  const uint_least16_t &value)> _lambda;
    std::function<uint_least16_t(CPU& cpu,  const uint_least16_t &instructionData)> _addressingLambda;    

public:    
    Instruction();
    Instruction(const AddressingMode &addressingMode, 
                const uint_least8_t &opcode,                 
                const string &menmonic,
                const AffectFlags &&affectedFlags = AffectFlags::None,
                const bool &readsFromMemory = false);
    
    Instruction(const AddressingMode &addressingMode, 
                const uint_least8_t &opcode,                 
                const uint_least8_t &length,                 
                const string &menmonic,
                const AffectFlags &&affectedFlags = AffectFlags::None,
                const bool &readsFromMemory = false);

    ~Instruction();    

    static void registrate(std::string const & name, create_f * fp);
    static void registrate2(std::string const & name, create_f2 * fp);    
    //static std::vector<std::shared_ptr<Instruction>> instantiate(std::string const & name);
    static void instantiateAll(std::vector<unique_ptr<Instruction>> &instructions);    
    static void instantiateAll(std::vector<Instruction> &instructions);    

    template <typename D>
    struct Registrar
    {
        explicit Registrar(std::string const & name)
        {
            Instruction::registrate(name, &D::createInstructions);
        }        
    };

    template <typename D>
    struct Registrar2
    {
        explicit Registrar2(std::string const & name)
        {
            Instruction::registrate2(name, &D::createInstructions2);
        }        
    };

    uint_least8_t getOpcode() const;
    uint_least8_t getLength() const;
    string getMenmonic() const;
    bool readsFromMemory() const;
    void setReadsFromMemory(bool readsFromMemory);
    void setLambda(std::function<uint_least16_t(CPU&,const uint_least16_t &)> lambda);
    void setAddressingLambda(std::function<uint_least16_t(CPU&,const uint_least16_t &)> lambda);

    
    void execute(CPU& cpu,  const uint_least16_t &instructionData);
    void printAddress(CPU& cpu,  const uint_least16_t &instructionData) const;
    virtual uint_least16_t action(CPU& cpu,  const uint_least16_t &value);    
    virtual void updateCarry(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
    virtual void updateZero(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
    virtual void updateInterruptDisabled(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
    virtual void updateDecimalMode(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
    virtual void updateOverflow(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
    virtual void updateNegative(CPU& cpu, const uint_least16_t &value, const uint_least16_t &actionValue);
};

class ClearInstruction : public Instruction {
public:
    using Instruction::Instruction;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    uint_least16_t action(CPU& cpu, const uint_least16_t &value);
};

class SetInstruction : public Instruction {
public:
    using Instruction::Instruction;
    static uint_least16_t sharedAction(CPU& cpu, const uint_least16_t &value);
    uint_least16_t action(CPU& cpu, const uint_least16_t &value);
};

#endif