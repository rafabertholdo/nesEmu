#include "SEDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
    Instruction::Registrar<SEDInstruction> registrar("SEDInstruction");
}

vector<shared_ptr<Instruction>> SEDInstruction::createInstructions() {
    auto instruction = make_shared<SEDInstruction>(implict,0x38,1,"SED");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void SEDInstruction::execute(CPU& cpu, const uint_least16_t &value) {    
    Instruction::execute(cpu, value);        
    cpu.Flags.DecimalMode = 1;
}
