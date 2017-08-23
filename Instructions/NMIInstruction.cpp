#include "Instructions/NMIInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<NMIInstruction> registrar("NMIInstruction");
}

void NMIInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x12;    
    insctructions[opcode] = Instruction(implict, opcode, "NMI", NMIInstruction::sharedAction);
}

uint_least16_t NMIInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    cpu.tick();
    cpu.tick();

    cpu.PC -= 1; //side effect of an interrupt be an instruction
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);                        
    cpu.push(cpu.Flags.raw);

    cpu.Flags.Break = 0; //Clear 4 is set on break
    
    cpu.tick();
    cpu.tick();
    cpu.tick();

    cpu.nmiEdgeDetected = true;
    return cpu.PC = cpu.getNmiVectorValue();      
}