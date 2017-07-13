#include "CLDInstruction.h"
#include <iostream>
#include <iomanip>

using namespace std;

vector<shared_ptr<Instruction>> CLDInstruction::createInstructions() {
    auto instruction = make_shared<CLDInstruction>(implict,0xD8,1,"CLD");
    vector<shared_ptr<Instruction>> result{instruction};
    return result;
}

void CLDInstruction::execute(CPU& cpu, const uint_least16_t &value) {
    Instruction::execute(cpu, value);
    cpu.Flags.DecimalMode = 0;
    cout << std::endl;
}