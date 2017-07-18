#include "Instructions/TXSInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<TXSInstruction> registrar("TXSInstruction");
}

vector<shared_ptr<Instruction>> TXSInstruction::createInstructions() {
    auto instruction = make_shared<TXSInstruction>(implict,0x9A,1,"TXS");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

uint_least16_t TXSInstruction::action(CPU& cpu, const uint_least16_t &value) {    
    return cpu.SP = cpu.X;
}