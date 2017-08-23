#include "Instructions/RTIInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<RTIInstruction> registrar("RTIInstruction");
}

void RTIInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x40;
    insctructions[opcode] = Instruction(implict, opcode, "RTI", RTIInstruction::sharedAction);
}

uint_least16_t RTIInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    uint_least8_t flagsToPop = cpu.pop();
    flagsToPop = (flagsToPop & ~0x30) | (cpu.Flags.raw & 0x30); //ignore bits 4 and 5
    cpu.Flags.raw = flagsToPop;
    return cpu.PC = cpu.pop() + (cpu.pop() << 8); 
}