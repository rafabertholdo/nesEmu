#include "Instructions/BRKInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
namespace
{
    Instruction::Registrar<BRKInstruction> registrar("BRKInstruction");
}
*/

vector<shared_ptr<Instruction>> BRKInstruction::createInstructions() {
    auto instruction = make_shared<BRKInstruction>(implict,0x00,1,"BRK");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t BRKInstruction::action(CPU& cpu, const uint_least16_t &value) {        
    cpu.push(cpu.PC  >> 8);
    cpu.push(cpu.PC);
    cpu.push(cpu.Flags.raw);
    
    cpu.Flags.InterruptDisabled = 1;        
    
    return cpu.PC = cpu.getBrkVectorValue();
}