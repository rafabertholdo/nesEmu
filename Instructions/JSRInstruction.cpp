#include "JSRInstruction.h"
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

void JSRInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    //store the PC on the stack
    cpu.push(cpu.Flags.raw);
    u16 addressToPush = cpu.PC + length - 1;    
    cpu.push(addressToPush >> 8);
    cpu.push(addressToPush);
    cpu.PC = value;
}