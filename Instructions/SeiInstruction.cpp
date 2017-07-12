#include "SeiInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

SEIInstruction::SEIInstruction() {
    opcode = 0x78;
    length = 1;   
    menmonic = "SEI";
}

void SEIInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {    
    Instruction::execute(cpu, instructionData);        
    cpu.Flags.InterruptEnabled = 0;
    cout << std::endl;
}
