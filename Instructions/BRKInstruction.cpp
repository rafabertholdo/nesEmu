#include "Instructions/BRKInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
namespace
{
    Instruction::Registrar<BRKInstruction> registrar("BRKInstruction");
    Instruction::Registrar2<BRKInstruction> registrar2("BRKInstruction");
}
*/

void BRKInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x00;    
    insctructions.at(opcode) = make_unique<BRKInstruction>(implict, opcode, "BRK");
}

void BRKInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x00;    
    auto instruction = BRKInstruction(implict, opcode, "BRK");
    instruction.setLambda(BRKInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BRKInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);
    cpu.push(cpu.Flags.raw);
    
    cpu.Flags.InterruptDisabled = 1;        
    
    return cpu.PC = cpu.getBrkVectorValue();
}

uint_least16_t BRKInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return BRKInstruction::sharedAction(cpu, value);
}