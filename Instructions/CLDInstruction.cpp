#include "CLDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

vector<shared_ptr<Instruction>> CLDInstruction::createInstructions() {
    auto instruction = make_shared<CLDInstruction>(noAddressing,0xD8,1,"CLD");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void CLDInstruction::execute(CPU& cpu, const vector<uint_least8_t> &instructionData) {
    Instruction::execute(cpu, instructionData);
    cpu.Flags.DecimalMode = 0;
    cout << std::endl;
}