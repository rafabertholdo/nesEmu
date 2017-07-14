#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "CPU.h"
#include "Addressing.h"
#include <unordered_map>

class CPU; //forward declaration


class Instruction {    
    using create_f = std::vector<std::shared_ptr<Instruction>>();    

    static std::unordered_map<std::string, create_f *> & registry();    
public:
    uint_least8_t length;
    uint_least8_t opcode;
    AddressingMode addressingMode;
    string menmonic;

    Instruction(const AddressingMode &addressingMode, const uint_least8_t &opcode, const uint_least8_t &length, const string &menmonic);    
    
    virtual void execute(CPU& cpu,  const uint_least16_t &value) = 0;

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

#endif