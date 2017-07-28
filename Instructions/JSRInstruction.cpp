#include "Instructions/JSRInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<JSRInstruction> registrar("JSRInstruction");
    Instruction::Registrar2<JSRInstruction> registrar2("JSRInstruction");
}

void JSRInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x20;    
    insctructions.at(opcode) = make_unique<JSRInstruction>(absolute, opcode, "JSR");
}

void JSRInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x20;    
    auto instruction = JSRInstruction(absolute, opcode, "JSR");
    instruction.setLambda(JSRInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t JSRInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {    
    //store the PC on the stack    
    cpu.tick();
    u16 addressToPush = cpu.PC - 1;    
    cpu.push(addressToPush >> 8);
    cpu.push(addressToPush);
    
    return cpu.PC = value;
}

uint_least16_t JSRInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return JSRInstruction::sharedAction(cpu, value);
}