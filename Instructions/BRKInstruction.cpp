#include "Instructions/BRKInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
namespace
{
    Instruction::Registrar<BRKInstruction> registrar("BRKInstruction");
}
*/

void BRKInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x00;    
    insctructions[opcode] = Instruction(implict, opcode, "BRK", BRKInstruction::sharedAction);
}

uint_least16_t BRKInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);
    cpu.push(cpu.Flags.raw);
    
    cpu.Flags.InterruptDisabled = 1;        
    
    return cpu.PC = cpu.getBrkVectorValue();
}