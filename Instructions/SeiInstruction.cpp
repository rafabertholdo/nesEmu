#include "SEIInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

vector<shared_ptr<Instruction>> SEIInstruction::createInstructions() {
    auto instruction = make_shared<SEIInstruction>(noAddressing,0x78,1,"SEI");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void SEIInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {    
    Instruction::execute(cpu, instructionData);        
    cpu.Flags.InterruptEnabled = 0;
    cout << std::endl;
}
