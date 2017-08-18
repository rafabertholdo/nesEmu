#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Addressing.h"
#include "RegBit.h"
#include <unordered_map>
#include <functional>
#include <tuple>
#include <map>
#include <string>

using namespace std;

class CPU; //forward declaration

typedef u16(*getAddressFunctionPointer_t)(CPU&, const u16&);
typedef u16(*actionFunctionPointer_t)(CPU&, const u16&);

class Instruction;
using InstructionArray = array<Instruction, 256>;

enum class AffectFlags : u8 {
    None = 0,
    Carry = 1,
    Zero = 2,
    InterruptDisabled = 4,
    DecimalMode = 8,
    Overflow = 64,
    Negative = 128
};

inline AffectFlags operator|(AffectFlags a, AffectFlags b) {
    return static_cast<AffectFlags>(static_cast<u8>(a) | static_cast<u8>(b));
}

class Instruction {        
    using create_f = void(InstructionArray &instrucitons);
    
    union {
        u8 raw;
        RegBit<0> Carry; // carry
        RegBit<1> Zero; // zero
        RegBit<2> InterruptDisabled; // interrupt enable/disable
        RegBit<3> DecimalMode; // decimal mode (unsupported on NES, but flag exists)
        // 4,5 (0x10,0x20) don't exist
        RegBit<6> Overflow; // overflow
        RegBit<7> Negative; // negative
    } m_affectedFlags;

    static unordered_map<string, create_f *> & registry();        
    static const map<AddressingMode, 
		tuple<u8, getAddressFunctionPointer_t>> addressingModes;
    static map<AddressingMode,
		tuple<u8, getAddressFunctionPointer_t>> createAddressingMap();
    void changeFlags(CPU& cpu, const u16 &value, const u16 &actionValue);
protected:
    u8 m_opcode;
    string m_menmonic;
    bool m_readsFromMemory;        
    u8 m_length;
    actionFunctionPointer_t m_actionFunctionPointer;
    getAddressFunctionPointer_t m_getAddressFunctionPointer;    

public:    
    Instruction();
    Instruction(const AddressingMode &addressingMode, 
                const u8 &opcode,                 
                const string &menmonic,
                const actionFunctionPointer_t &m_actionFunctionPointer,
                const AffectFlags &&affectedFlags = AffectFlags::None,
                const bool &readsFromMemory = false);
    ~Instruction();    

    static void registrate(const string &name, create_f *fp);        
    static void instantiateAll(InstructionArray &instructions);    

    template <typename D>
    struct Registrar
    {
        explicit Registrar(const string &name)
        {
            Instruction::registrate(name, &D::createInstructions);
        }        
    };    

    const u8& opcode() const;
    const u8& length() const;
    const string& menmonic() const;
    const bool& readsFromMemory() const;
    void readsFromMemory(const bool &readsFromMemory, const AddressingMode &addressingMode);
    
    void execute(CPU& cpu,  const u16 &instructionData);    
    virtual u16 action(CPU& cpu,  const u16 &value);    
    virtual void updateCarry(CPU& cpu, const u16 &value, const u16 &actionValue);
    virtual void updateZero(CPU& cpu, const u16 &value, const u16 &actionValue);
    virtual void updateInterruptDisabled(CPU& cpu, const u16 &value, const u16 &actionValue);
    virtual void updateDecimalMode(CPU& cpu, const u16 &value, const u16 &actionValue);
    virtual void updateOverflow(CPU& cpu, const u16 &value, const u16 &actionValue);
    virtual void updateNegative(CPU& cpu, const u16 &value, const u16 &actionValue);
};

class ClearInstruction : public Instruction {
public:
    using Instruction::Instruction;
    static u16 sharedAction(CPU& cpu, const u16 &value);
};

class SetInstruction : public Instruction {
public:
    using Instruction::Instruction;
    static u16 sharedAction(CPU& cpu, const u16 &value);
};

#endif