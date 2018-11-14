#include "Instructions/RSTInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RSTInstruction> registrar("RSTInstruction");
}

void RSTInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x22;    
    insctructions[opcode] = Instruction(implict, opcode, "RST", RSTInstruction::sharedAction);
}

uint_least16_t RSTInstruction::sharedAction(CPU& cpu, const uint_least16_t &) {        
    cpu.tick();
    cpu.tick();
    return cpu.PC = cpu.getResetVectorValue();    
}
