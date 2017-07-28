#include "Instructions/TSXInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TSXInstruction> registrar("TSXInstruction");
    Instruction::Registrar2<TSXInstruction> registrar2("TSXInstruction");
}

void TSXInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0xBA;    
    insctructions.at(opcode) = make_unique<TSXInstruction>(implict, opcode, "TSX", AffectFlags::Negative | AffectFlags::Zero);
}

void TSXInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0xBA;    
    auto instruction = TSXInstruction(implict, opcode, "TSX", AffectFlags::Negative | AffectFlags::Zero);
    instruction.setLambda(TSXInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t TSXInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    cpu.X = cpu.SP;
    cpu.tick();
    return cpu.X;
}

uint_least16_t TSXInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return TSXInstruction::sharedAction(cpu, value);
}