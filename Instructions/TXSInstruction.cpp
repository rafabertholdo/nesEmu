#include "Instructions/TXSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TXSInstruction> registrar("TXSInstruction");
    Instruction::Registrar2<TXSInstruction> registrar2("TXSInstruction");
}

void TXSInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x9A;    
    insctructions.at(opcode) = make_unique<TXSInstruction>(implict, opcode, "TXS");
}

void TXSInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x9A;    
    auto instruction = TXSInstruction(implict, opcode, "TXS");
    instruction.setLambda(TXSInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TXSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.SP = cpu.X;
    cpu.tick();
    return cpu.SP;
}

uint_least16_t TXSInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TXSInstruction::sharedAction(cpu, value);
}