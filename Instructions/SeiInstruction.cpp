#include "SEIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

vector<shared_ptr<Instruction>> SEIInstruction::createInstructions() {
    auto instruction = make_shared<SEIInstruction>(implict,0x78,1,"SEI");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void SEIInstruction::execute(CPU& cpu, const uint_least16_t &value) {    
    Instruction::execute(cpu, value);        
    cpu.Flags.InterruptEnabled = 0;
    cout << std::endl;
}
