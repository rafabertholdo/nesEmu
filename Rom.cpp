#include "Rom.h" 
#include "Utils.cpp" 
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

#include <cassert>

static const int PROGRAM_BLOCK_SIZE_IN_KB = 0x4000; //16kb
static const int VRAM_BLOCK_SIZE_IN_KB = 0x2000; //8kb

class Rom::impl {
public:
    vector<BYTE> data;
    vector<BYTE> rom;    
    vector<BYTE> vram;    
    BYTE numberOf16kbRomBanks;
    BYTE numberOf8kbVRomBanks;
    BYTE ctrlbyte;
    BYTE mappernum;
    
    void loadProgramData() {
        int programLength = (PROGRAM_BLOCK_SIZE_IN_KB * numberOf16kbRomBanks);
        int programStart = HEADER_SIZE;
        int programEnd = programStart + programLength;
        
        int vramLength = (VRAM_BLOCK_SIZE_IN_KB * numberOf8kbVRomBanks);
        int vramStart = programEnd + 1;
        int vramEnd = vramStart + vramLength;
        
        rom = vector<BYTE>(&data[programStart], &data[programEnd]);
        //vram = vector<BYTE>(&data[vramStart], &data[vramEnd]);

        std::cout << "program length: " << programLength << std::endl;
        std::cout << "varam length: " << vramLength << std::endl;
    }

    bool readFile(const char* filename) {
        // open the file:
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            // Stop eating new lines in binary mode!!!
            file.unsetf(std::ios::skipws);

            // get its size:
            std::streampos fileSize;

            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            // reserve capacity
            data.reserve(fileSize);

            // read the data:
            data.insert(data.begin(),
                    std::istream_iterator<BYTE>(file),
                    std::istream_iterator<BYTE>());
            return true;
        }
        return false;
    }
};

Rom::Rom(const char* filePath): pimpl{std::make_unique<impl>()} {
    std::cout << "Reading file: " << filePath << std::endl;
    if (pimpl->readFile(filePath)) {
        // Read the ROM file header
        assert(pimpl->data[0] =='N' && pimpl->data[1]=='E' && pimpl->data[2]=='S' && pimpl->data[3]=='\32');

        pimpl->numberOf16kbRomBanks = pimpl->data[4];        
        pimpl->numberOf8kbVRomBanks = pimpl->data[5];
        pimpl->ctrlbyte = pimpl->data[6];
        pimpl->mappernum = pimpl->data[7] | (pimpl->ctrlbyte>>4); //Four higher bits of ROM Mapper Type
        if(pimpl->mappernum >= 0x40) {
            pimpl->mappernum &= 15;
        }        

        pimpl->loadProgramData();
    }
    
    std::cout << "numberOfProgramBlocks: " << static_cast<int>(pimpl->numberOf16kbRomBanks) << std::endl;
    std::cout << "numberOfvramBlocks: " << static_cast<int>(pimpl->numberOf8kbVRomBanks) << std::endl;
}

Rom::~Rom(){
    
}

uint_least8_t Rom::access(const uint_least16_t &address, const uint_least8_t &value, const bool &write) {
    //0x8000 - 0xBFFF
    //0xC000 - 0xFFFF
    return pimpl->rom[static_cast<int>(address) - (address < 0xC000 ? 0x8000 : 0xC000)];
}


/*
uint_least8_t Rom::load(uint_least8_t address) const {
    return pimpl->data[address];
}

vector<uint_least8_t> Rom::load(uint_least8_t address, uint_least8_t length) const {
    vector<uint_least8_t> sub(&pimpl->data[address],&pimpl->data[address + length]);    
    return sub;
}*/