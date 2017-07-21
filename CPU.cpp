#include "CPU.h" 
#include "PPU.h"
#include "Utils.cpp"

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <cassert>
#include <SDL2/SDL.h>


using namespace std;

CPU::CPU(const shared_ptr<IO> &io): RAM(0x800) { //2k of ram      
    CPU::io = io;
    for(auto&& instruction : Instruction::instantiateAll()) {
        instructionsMapping[instruction->opcode] = instruction;
    }
    
    testing = false;
    
    Flags.raw = 0x24;
    SP = 0xFD;
    reset = false;

    cout << "RAM size: " << RAM.size() << endl;
}

CPU::CPU(const CPU &cpu) {
    A = cpu.A;
    X = cpu.X;
    Y = cpu.Y;
    SP = cpu.SP;
    Flags.raw = cpu.Flags.raw;
    PC = cpu.PC;
}

CPU::~CPU() {

}

u16 CPU::getNmiVectorValue() {
    return Utils<u8>::getLittleEndianValue(read(0xFFFA,2)); //nmi vector
}

u16 CPU::getResetVectorValue() {
    return Utils<u8>::getLittleEndianValue(read(0xFFFC,2)); //reset vector
}

u16 CPU::getBrkVectorValue() {
    return Utils<u8>::getLittleEndianValue(read(0xFFFE,2)); //brk vector
}

void CPU::loadRom(const shared_ptr<ROM> &rom) {
    CPU::rom = rom;
    if (testing) {
        PC = 0xC000;    
    } else {
        PC = getResetVectorValue();
    }
}   

void CPU::test(const string &line, const vector<uint_least8_t> &instructionData, const string &menmonic) {
    std::regex rgx("(.{4})\\s*(.{9}).(.{3})\\s(.{28})A:(.{2})\\sX:(.{2})\\sY:(.{2})\\sP:(.{2})\\sSP:(.{2})\\sCYC:(.*)");            
    std::smatch matches;

    if(std::regex_search(line, matches, rgx)) {       

        u16 expectedPC = stoul(matches[1].str(),nullptr,16);
        string expectedData =  matches[2].str();
        
        string expectedInstruction = matches[3].str();
        string expectedInstructionData = matches[4].str();                
        u8 expectedA = stoul(matches[5].str(),nullptr,16);                
        u8 expectedX = stoul(matches[6].str(),nullptr,16);                
        u8 expectedY = stoul(matches[7].str(),nullptr,16);                
        u8 expectedP = stoul(matches[8].str(),nullptr,16);                
        u8 expectedSP = stoull(matches[9].str(),nullptr,16);   

        assert(PC == expectedPC);
        assert(A == expectedA);
        assert(X == expectedX);
        assert(Y == expectedY);
        assert(Flags.raw == expectedP);
        assert(SP == expectedSP);
        assert(menmonic.compare(expectedInstruction) == 0);
    } else {
        std::cout << "Match not found\n";
    }
}

void CPU::identify(const vector<uint_least8_t> &instructionData, const shared_ptr<Instruction> &instruction) {
    Utils<uint_least16_t>::printHex(PC);
    cout << "  ";
    Utils<uint_least8_t>::printHex(instruction->opcode);        
    cout << " ";

    int verboseData = 7;
    for (auto data : instructionData) {
        Utils<uint_least8_t>::printHex(data);
        cout << " ";
        verboseData -= 3;
    }
    cout << std::setw(verboseData) << std::setfill(' ') << " ";    
    std::cout << instruction->menmonic << " ";
    instruction->printAddress(*this, instructionData);
    dumpRegs();
}

void CPU::run() {   

    std::ifstream testLog("build/nestest.log.txt");

    bool quit = false;
    //Event handler
    SDL_Event event;
    while(!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &event ) != 0 ) {
            switch( event.type ){
                case SDL_KEYDOWN:
                    //Set the proper message surface
                    switch( event.key.keysym.sym ) {
                        case SDLK_j: io->Joy1Write(io->JoyRead(0) | 1 << 7); break;
                        case SDLK_h: io->Joy1Write(io->JoyRead(0) | 1 << 6); break;
                        case SDLK_q: io->Joy1Write(io->JoyRead(0) | 1 << 5); break;
                        case SDLK_e: io->Joy1Write(io->JoyRead(0) | 1 << 4); break;
                        case SDLK_w: io->Joy1Write(io->JoyRead(0) | 1 << 3); break;
                        case SDLK_s: io->Joy1Write(io->JoyRead(0) | 1 << 2); break;
                        case SDLK_a: io->Joy1Write(io->JoyRead(0) | 1 << 1); break;
                        case SDLK_d: io->Joy1Write(io->JoyRead(0) | 1 << 0); break;
                    }
                    break;

                case SDL_KEYUP:
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_j: io->Joy1Write(io->JoyRead(0) & ~(1 << 7)); break;
                        case SDLK_h: io->Joy1Write(io->JoyRead(0) & ~(1 << 6)); break;
                        case SDLK_q: io->Joy1Write(io->JoyRead(0) & ~(1 << 5)); break;
                        case SDLK_e: io->Joy1Write(io->JoyRead(0) & ~(1 << 4)); break;
                        case SDLK_w: io->Joy1Write(io->JoyRead(0) & ~(1 << 3)); break;
                        case SDLK_s: io->Joy1Write(io->JoyRead(0) & ~(1 << 2)); break;
                        case SDLK_a: io->Joy1Write(io->JoyRead(0) & ~(1 << 1)); break;
                        case SDLK_d: io->Joy1Write(io->JoyRead(0) & ~(1 << 0)); break;
                    }                    
                    break;
                
                case SDL_QUIT:
                    quit = true;
                default:
                    break;
            }            
        }  

        string line;
        if (testing) {        
            if (testLog.is_open()) {
                if (!getline(testLog,line) ) {
                    break;
                } 
            }
            else { 
                cout << "Unable to open file"; 
            }       
        }               
        
        /* Check the state of NMI flag */
        bool nmi_now = nmi;

        unsigned instructionCode = read(PC);        
        
        bool executeInstruction = true;
        if(reset)  { 
            tick();
            tick();
            PC = getResetVectorValue();
            executeInstruction = false;            
        } else if(nmi_now && !nmi_edge_detected) {             
            tick();
            tick();
            push(PC  >> 8);
            push(PC);                        
            push(Flags.raw);
            Flags.Break = 0; //Clear 4 is set on break
            tick();
            tick();
            tick();

            PC = getNmiVectorValue();
            nmi_edge_detected = true; 
            executeInstruction = false;
        } else if(intr && !Flags.InterruptDisabled) {             
            tick();
            push(PC  >> 8);
            push(PC);
            push(Flags.raw);
            Flags.Break = 1; //bit 4 is set on break
            Flags.InterruptDisabled = 1;
            tick();
            tick();
            tick();

            PC = getBrkVectorValue();
            executeInstruction = false;
        }
        
        if(!nmi_now) { 
            nmi_edge_detected=false;
        }

        if (executeInstruction && instructionsMapping.find(instructionCode) != instructionsMapping.end()) {
            shared_ptr<Instruction> instruction = instructionsMapping[instructionCode]; //fetch instruction
            
            vector<uint_least8_t> instructionData;
            if (instruction->length > 1) {
                instructionData = read(PC+1, instruction->length - 1);            
            }
            
            
            if (testing) {                
                identify(instructionData, instruction);            
                test(line, instructionData, instruction->menmonic);                
            }
            
            PC += instruction->length;             
            instruction->execute(*this, instructionData); 
        }   
        reset = false;
    }	
}


uint_least8_t CPU::memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write)
{
    // Memory writes are turned into reads while reset is being signalled
    if(reset && write) return memAccess(address, 0, false);

    tick();
    if (address < 0x2000) { 
        uint_least8_t& reference = RAM[address & 0x7FF]; 
        if(!write)
            return reference; 
        reference = value; 
    } else if (address < 0x4000) {
        return ppu->access(address&7, value, write);
    }
    else if (address < 0x4018)    
        switch(address & 0x1F)
        {
            case 0x14: // OAM DMA: Copy 256 bytes from RAM into PPU's sprite memory
                if(write) {
                    for (unsigned b=0; b<256; ++b) {
                        CPU::write(0x2004, read((value&7)*0x0100+b));
                    }
                }
                return 0;
            case 0x15: 
                break;
                /*
                if(!write) {
                    return APU::Read();
                }        
                APU::Write(0x15,value); break;*/
            case 0x16: //joy1
                if(!write) {
                    return io->JoyRead(false);                     
                }
                io->JoyStrobe(value); 
                break;
            case 0x17: //joy1 
                if(!write) {
                    return io->JoyRead(true);  // write:passthru
                }
            default: 
                break;
                /*
                if(!write) {
                    break;
                } 
               APU::Write(address&0x1F, value);*/
        }
    else return rom->prgAccess(address, value, write);
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
    //std::cout << " CYC:" << std::setw(3) << std::setfill(' ') << static_cast<int>(0) << std::endl; 
	cout << std::endl;
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

void CPU::setPPU(const shared_ptr<PPU> &ppu) {
    CPU::ppu = ppu;
}

void CPU::tick() {
    // PPU clock: 3 times the CPU rate
    for(unsigned n=0; n<3; ++n) {
        ppu->tick();
    }
    // APU clock: 1 times the CPU rate
    //for(unsigned n=0; n<1; ++n) APU::tick();
}