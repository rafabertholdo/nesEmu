#include "Instructions/BRKInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BRKInstruction> registrar("BRKInstruction");
}

void BRKInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x02;    
    insctructions[opcode] = Instruction(implict, opcode, "BRK", BRKInstruction::sharedAction);
}

uint_least16_t BRKInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {        
    cpu.tick();
    
    cpu.PC -= 1; //side effect of an interrupt be an instruction
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);
    cpu.push(cpu.Flags.raw);

    cpu.Flags.Break = 1; //bit 4 is set on break
    cpu.Flags.InterruptDisabled = 1;

    cpu.tick();
    cpu.tick();
    cpu.tick();

    return cpu.PC = cpu.getBrkVectorValue();
}
