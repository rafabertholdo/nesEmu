#include "Instructions/RTSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<RTSInstruction> registrar("RTSInstruction");
    Instruction::Registrar2<RTSInstruction> registrar2("RTSInstruction");
}

void RTSInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x60;    
    insctructions.at(opcode) = make_unique<RTSInstruction>(implict, opcode, "RTS");
}

void RTSInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x60;    
    auto instruction = RTSInstruction(implict, opcode, "RTS");
    instruction.setLambda(RTSInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t RTSInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {        
    cpu.tick();
    cpu.tick();
    cpu.PC = cpu.pop() + (cpu.pop() << 8) + 1;
    cpu.tick();
    return cpu.PC;
}

uint_least16_t RTSInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    return RTSInstruction::sharedAction(cpu, value);
}