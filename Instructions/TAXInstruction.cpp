#include "Instructions/TAXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TAXInstruction> registrar("TAXInstruction");
    Instruction::Registrar2<TAXInstruction> registrar2("TAXInstruction");
}

void TAXInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xAA;    
    insctructions.at(opcode) = make_unique<TAXInstruction>(implict, opcode, "TAX", AffectFlags::Negative | AffectFlags::Zero);
}

void TAXInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xAA;    
    auto instruction = TAXInstruction(implict, opcode, "TAX", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(TAXInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TAXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X = cpu.A;
    cpu.tick();
    return cpu.X;
}

uint_least16_t TAXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TAXInstruction::sharedAction(cpu, value);
}