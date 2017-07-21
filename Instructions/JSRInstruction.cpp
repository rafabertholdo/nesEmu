#include "Instructions/JSRInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<JSRInstruction> registrar("JSRInstruction");
}

vector<shared_ptr<Instruction>> JSRInstruction::createInstructions() {
    auto instruction = make_shared<JSRInstruction>(absolute,0x20,3,"JSR");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t JSRInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    //store the PC on the stack    
    cpu.tick();
    u16 addressToPush = cpu.PC - 1;    
    cpu.push(addressToPush >> 8);
    cpu.push(addressToPush);
    
    return cpu.PC = value;
}