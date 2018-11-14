#include "Instructions/JSRInstruction.h"
#include "CPU.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace {
    Instruction::Registrar<JSRInstruction> registrar("JSRInstruction");
}

void JSRInstruction::createInstructions(InstructionArray &insctructions) {    
    auto opcode = 0x20;        
    insctructions[opcode] = Instruction(absolute, opcode, "JSR", JSRInstruction::sharedAction);
}

uint_least16_t JSRInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    //store the PC on the stack    
    cpu.tick();
    u16 addressToPush = cpu.PC - 1;    
    cpu.push(addressToPush >> 8);
    cpu.push(addressToPush);
    
    return cpu.PC = value;
}