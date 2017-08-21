#include "CPU.h" 
#include "PPU.h"
#include "APU.h"
#include "Utils.cpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <cassert>
#include <SDL.h>

using namespace std;

CPU::CPU(const shared_ptr<IO> &io): RAM(0x800) , totalCycles(29780)  { //2k of ram      
    CPU::io = io;  
    
    Instruction::instantiateAll(instructions);       
    
    testing = false;
    if (testing) {
        _testLogFile = std::ifstream("build/nestest.log.txt");
    }
	remainingCycles = totalCycles;
    Flags.raw = 0x24;
    SP = 0xFD;
    reset = false;
    running = true;
    tickCount = 0;
    cout << "RAM size: " << RAM.size() << endl;
}

CPU::CPU(const CPU &cpu): totalCycles(29781) {
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
    return read(0xFFFA,2); //nmi vector
}

u16 CPU::getResetVectorValue() {
    return read(0xFFFC,2); //reset vector
}

u16 CPU::getBrkVectorValue() {
    return read(0xFFFE,2); //brk vector
}

void CPU::loadRom(const shared_ptr<ROM> &rom) {
    auto prg = rom->prg();
    std::copy_n(prg.begin(), prg.size(), m_prg.begin());    
    m_prgMap = rom->prgMap();

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

		auto expectedPC = stoul(matches[1].str(),nullptr,16);
        string expectedData =  matches[2].str();
        
        string expectedInstruction = matches[3].str();
        string expectedInstructionData = matches[4].str();                
        auto expectedA = stoul(matches[5].str(),nullptr,16);                
		auto expectedX = stoul(matches[6].str(),nullptr,16);
		auto expectedY = stoul(matches[7].str(),nullptr,16);
		auto expectedP = stoul(matches[8].str(),nullptr,16);
		auto expectedSP = stoull(matches[9].str(),nullptr,16);

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

void CPU::identify(const vector<uint_least8_t> &instructionData, const Instruction &instruction) {
    Utils<uint_least16_t>::printHex(PC);
    cout << "  ";
    Utils<uint_least8_t>::printHex(instruction.opcode());        
    cout << " ";

    int verboseData = 7;
    for (auto data : instructionData) {
        Utils<uint_least8_t>::printHex(data);
        cout << " ";
        verboseData -= 3;
    }
    cout << std::setw(verboseData) << std::setfill(' ') << " ";    
    std::cout << instruction.menmonic() << " ";    
    dumpRegs();
}

bool CPU::handleInterruptions() {
    bool executeInstruction = true;

    /* Check the state of NMI flag */
    bool nmi_now = nmi;

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
        nmi_edge_detected = false;
    }
    return executeInstruction;
}

void CPU::executeInstruction(Instruction &instruction) {
    u16 instructionData = 0;
    auto length = instruction.length();
    if (length > 1) {
        instructionData = read(PC+1, instruction.length() - 1);            
    }
    
    if (testing) {               
        string line;
        if (testing) {        
            if (_testLogFile.is_open()) {
                getline(_testLogFile,line);
            } else { 
                cout << "Unable to open file"; 
            }       
        }
        
        vector<u8> instructionDataVector;
        if (instruction.length() > 1) {
            for(int i=0;i<instruction.length() - 1;i++) {
                instructionDataVector.push_back(read(PC+1 +i));
            }
        }

        identify(instructionDataVector, instruction);   
        test(line, instructionDataVector, instruction.menmonic());   

        
    }
    /*
    executedInstructionsCount++;
    if (timer.elapsed() > 1000) {
        std::cout << executedInstructionsCount << " i/s" << std::endl;
        executedInstructionsCount = 0;
        timer.reset();
    }
    */
    
    PC += instruction.length();             
    instruction.execute(*this, instructionData); 
}

void CPU::run() {   

    bool quit = false;
    //Event handler    
    remainingCycles += totalCycles;
    while(remainingCycles > 0) {

        auto instructionCode = read(PC);        
        bool willExecuteInstruction = handleInterruptions();

        if (willExecuteInstruction) {			
            executeInstruction(instructions[instructionCode]);           
        }   
        reset = false;
    }	
}

void CPU::dmaOam(const uint_least8_t &value) {
    for (unsigned b=0; b<256; ++b) {
        write(0x2004, read((value&7)*0x0100+b));
    }
}

uint_least8_t CPU::memAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write) {
    // Memory writes are turned into reads while reset is being signalled
    if(reset && write) return memAccess(address, 0, false);
    
    tick();
	if (address < 0x2000) {  // RAM 0x0000 ... 0x1FFF
		uint_least8_t& reference = RAM[address & 0x7FF];
		if (!write) {
			return reference;
		}
		reference = value;
	} else if (address < 0x4000) { //PPU 0x2000 ... 0x3FFF
		return ppu->access(address & 7, value, write);
	} else if (address < 0x4014) { //APU address 0x4000 ... 0x4013
		if(write) {
            _apu->write(address & 0x1F, value);
        }
	} else if (address == 0x4014) { // OAM DMA.
		if (write) {
            dmaOam(value);
        }
	} else if (address == 0x4015) {
        if(write) {
            _apu->write(0x15,value);
        } else {
            return _apu->read(); 
        }
    } else if (address == 0x4016) { // Joypad 0.
		if (write) { 
			io->JoyStrobe(value);        
		} else { 
			return io->JoyRead(0);        
		}
	} else if (address == 0x4017) { // Joypad 1.
		if (write) {
			_apu->write(0x17,value);
		} else {
            io->JoyRead(1);
        }
	} else { // ROM prg access 0x4018 ... 0xFFFF
		return prgAccess(address, value, write);
	}

    return 0;
}

u8 CPU::read(const u16 &address) {
    return memAccess(address, 0, false);
}

u16 CPU::read(const u16 &address, const u8 &length) {
    u16 result = 0;
    for (u8 i = 0; i < length; i++) {
        result |= read(address+i) << (i * 8); //read little endian value
    }
    
    return result;
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
void CPU::setAPU(const shared_ptr<APU> &apu) {
    _apu = apu;
}

void CPU::tick() {
    for(unsigned n=0; n<3; ++n) {
        ppu->tick();
    }
    _apu->tick();
    
    remainingCycles--;
}


uint_least8_t CPU::prgAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write) {
    //0x8000 - 0xBFFF
    //0xC000 - 0xFFFF

    if (address >= PRG_START) {
        auto page = (address / PRG_GRANULARITY) % PRG_PAGES;
        auto pageOffset = address % PRG_GRANULARITY;
        return m_prg[m_prgMap[page] + pageOffset];
    } else {
        //TODO: PRG RAM
        // return prgRam[addr - 0x6000];
        return 0;
    }   
}
