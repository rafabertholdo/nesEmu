#include "Rom.h" 
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

static const int KB_SIZE = 1024;
static const int PROGRAM_BLOCK_SIZE_IN_KB = 16;

class Rom::impl {
public:
    vector<BYTE> data;    
    vector<BYTE> programData;    
    int numberOfProgramBlocks;

    void loadProgramData() {
        int programLength = HEADER_SIZE + (PROGRAM_BLOCK_SIZE_IN_KB * KB_SIZE * numberOfProgramBlocks);
        std::cout << "program length: " << programLength << std::endl;
        programData = vector<BYTE>(&data[0], &data[programLength]);
    }

    void readFile(const char* filename) {
        // open the file:
        std::ifstream file(filename, std::ios::binary);

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
    }
};

Rom::Rom(const char* filePath): pimpl{std::make_unique<impl>()} {
    std::cout << "Reading file: " << filePath << std::endl;
    pimpl->readFile(filePath);
    pimpl->numberOfProgramBlocks = pimpl->data[4];
    pimpl->loadProgramData();
    std::cout << "numberOfProgramBlocks: " << pimpl->numberOfProgramBlocks << std::endl;
}

Rom::~Rom(){
    
}

uint_least8_t Rom::load(uint_least8_t address) const {
    return pimpl->data[address];
}

vector<uint_least8_t> Rom::load(uint_least8_t address, uint_least8_t length) const {
    vector<uint_least8_t> sub(&pimpl->data[address],&pimpl->data[address + length]);    
    return sub;
}