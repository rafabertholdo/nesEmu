#include "Instructions/BCCInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BCCInstruction> registrar("BCCInstruction");    
}

void BCCInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x90;            
    insctructions[opcode] = Instruction(relative, opcode, "BCC", BCCInstruction::sharedAction);
}

uint_least16_t BCCInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (!cpu.Flags.Carry) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}