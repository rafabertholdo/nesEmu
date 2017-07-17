#include "CPU.h" 
#include "Utils.cpp"

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

CPU::CPU(): RAM(0x800) { //2k of ram  
    
    for(auto&& instruction : Instruction::instantiateAll()) {
        instructionsMapping[instruction->opcode] = instruction;
    }
    
    PC = 0xC000;    
    Flags.raw = 0x24;
    SP = 0xFD;
    reset = true;

    cout << "RAM size: " << RAM.size() << endl;
}

CPU::~CPU() {

}

void CPU::loadRom(Rom &rom) {
    CPU::rom = &rom;
}

void CPU::run() {   
    while (true) {
        uint_least16_t pcStash = PC;
        uint_least8_t instructionCode = read(PC);
        
        Utils<uint_least16_t>::printHex(PC);
        cout << "  ";
        Utils<uint_least8_t>::printHex(instructionCode);        
        cout << " ";

        if (instructionsMapping.find(instructionCode) != instructionsMapping.end()) {
            shared_ptr<Instruction> instruction = instructionsMapping[instructionCode]; //fetch instruction
            
            int verboseData = 7;
            vector<uint_least8_t> instructionData;
            if (instruction->length > 1) {
                instructionData = read(PC+1, instruction->length - 1);

                for (auto data : instructionData) {
                    Utils<uint_least8_t>::printHex(data);
                    cout << " ";
                    verboseData -= 3;
                }                
            }
            PC += instruction->length; 
            cout << std::setw(verboseData) << std::setfill(' ') << " ";
            
            instruction->execute(*this, instructionData); //execute instruction            
            //increment program counter if instruction did't change it, aka jumps and branches
        } else {            
            std::cout << "Instruction " << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(instructionCode) << " not implemented" << std::endl;
            dumpRegs();
            return;            
        }
        reset = false;
    }
}


uint_least8_t CPU::memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write)
{
    // Memory writes are turned into reads while reset is being signalled
    if(reset && write) return memAccess(address, 0, false);

    //tick();
    if (address < 0x2000) { 
        uint_least8_t& reference = RAM[address & 0x7FF]; 
        if(!write)
            return reference; 
        reference = value; 
    } else if (address < 0x4000) {
        return ppu.access(address&7, value, write);
    }
    //else if (address < 0x4018)
        /*
        switch(address & 0x1F)
        {
            case 0x14: // OAM DMA: Copy 256 bytes from RAM into PPU's sprite memory
                if(write) {
                    for (unsigned b=0; b<256; ++b) {
                        write(0x2004, read((v&7)*0x0100+b));
                    }
                }
                return 0;
            case 0x15: if(!write) return APU::Read();        APU::Write(0x15,value); break;
            case 0x16: if(!write) return IO::JoyRead(false); IO::JoyStrobe(value); break;
            case 0x17: if(!write) return IO::JoyRead(true);  // write:passthru
            default: if(!write) break;
                     APU::Write(address&0x1F, value);
        }*/
    else return rom->access(address, value, write);
    return 0;
}

u8 CPU::read(const u16 &address) {
    return memAccess(address, 0, false);
}

vector<u8> CPU::read(const u16 &address, const u8 &length) {
    std::vector<u8> buffer;
    buffer.reserve(length);
    for (u8 i = 0; i < length; i++) {
        buffer.push_back(read(address+i));
    }
    
    return buffer;
}

void CPU::write(const uint_least16_t &address, const uint_least8_t &value) {
    memAccess(address, value, true);
}

void CPU::dumpRegs() {

    //A:00 X:00 Y:00 P:24 SP:FD CYC:  0
    std::cout << "A:";
    Utils<uint_least8_t>::printHex(A);
    std::cout << " X:";
    Utils<uint_least8_t>::printHex(X);
    std::cout << " Y:";
    Utils<uint_least8_t>::printHex(Y);
    std::cout << " P:";
    Utils<uint_least8_t>::printHex(Flags.raw);
    std::cout << " SP:";
    Utils<uint_least8_t>::printHex(SP);
    std::cout << " CYC:" << std::setw(3) << std::setfill(' ') << static_cast<int>(0) << std::endl; 
}

void CPU::push(const uint_least8_t &value) {
    write(SP + (1 << 8), value);
    SP-=1;
}

uint_least8_t CPU::pop() {
    SP+=1;
    u8 topElement = read(SP + (1 << 8));
    return topElement;
}