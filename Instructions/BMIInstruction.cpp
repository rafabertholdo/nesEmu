#include "Instructions/BMIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<BMIInstruction> registrar("BMIInstruction");
    Instruction::Registrar2<BMIInstruction> registrar2("BMIInstruction");
}

void BMIInstruction::createInstructions(vector<unique_ptr<Instruction>> &insctructions) {    
    auto opcode = 0x30;
    insctructions.at(opcode) = make_unique<BMIInstruction>(relative,opcode,"BMI");
}


void BMIInstruction::createInstructions2(vector<Instruction> &insctructions) {    
    auto opcode = 0x30;
    auto instruction = BMIInstruction(relative,opcode,"BMI");
    instruction.setLambda(BMIInstruction::sharedAction);
    insctructions.at(opcode) = instruction;
}

uint_least16_t BMIInstruction::sharedAction(CPU& cpu, const uint_least16_t &value) {
    if (cpu.Flags.Negative) {
        cpu.tick();
        cpu.PC = value;
    }
    return cpu.PC;
}

uint_least16_t BMIInstruction::action(CPU& cpu, const uint_least16_t &value) {
    return BMIInstruction::sharedAction(cpu, value);
}