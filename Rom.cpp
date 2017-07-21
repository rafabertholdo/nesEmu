#include "Rom.h" 
#include "Utils.cpp" 
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

#include <cassert>

static const int kPrgBlockSizeInKb = 0x4000; //16kb
static const int kChrBlockSizeInKb = 0x2000; //8kb

class ROM::impl {
public:
    vector<u8> data;
    vector<u8> rom;    
    vector<u8> chr;    
    u16 prgROMSize;
    u16 prgRAMSize;
    u16 chrROMSize;
    u8 ctrlbyte;
    u8 mappernum;        

    void loadProgramData() {
        int prgStart = HEADER_SIZE;
        int prgEnd = prgStart + prgROMSize - 1;
        
        int chrStart = prgEnd + 1;
        int chrEnd = chrStart + chrROMSize - 1;
        
        rom = vector<u8>(&data[prgStart], &data[prgEnd]);
        if (chrROMSize) {
            chr = vector<u8>(&data[chrStart], &data[chrEnd]);    
        }        

        std::cout << "prg length: " << prgROMSize << std::endl;
        std::cout << "chr length: " << chrROMSize << std::endl;
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
                    std::istream_iterator<u8>(file),
                    std::istream_iterator<u8>());
            return true;
        }
        return false;
    }
};

ROM::ROM(const char* filePath): pimpl{std::make_unique<impl>()} {   

    std::cout << "Reading file: " << filePath << std::endl;
    if (pimpl->readFile(filePath)) {
        // Read the ROM file header
        assert(pimpl->data[0] =='N' && pimpl->data[1]=='E' && pimpl->data[2]=='S' && pimpl->data[3]=='\32');

        pimpl->prgROMSize = pimpl->data[4] * kPrgBlockSizeInKb; 
        pimpl->chrROMSize = pimpl->data[5] * kChrBlockSizeInKb;
        pimpl->ctrlbyte = pimpl->data[6];
        pimpl->mappernum = pimpl->data[7] | (pimpl->ctrlbyte>>4); //Four higher bits of ROM Mapper Type
        if(pimpl->mappernum >= 0x40) {
            pimpl->mappernum &= 15;
        }        
        pimpl->prgRAMSize = pimpl->data[8] * kChrBlockSizeInKb;

        pimpl->loadProgramData();
        init();        
    }
}

ROM::~ROM(){
    
}

uint_least8_t ROM::prgAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write) {
    //0x8000 - 0xBFFF
    //0xC000 - 0xFFFF
    if (pimpl->prgROMSize == 1) {
        return pimpl->rom[static_cast<int>(address) - (address < 0xC000 ? 0x8000 : 0xC000)];
    } else {
        return pimpl->rom[static_cast<int>(address) - 0x8000];
    }
    
}

template<unsigned npages,unsigned char*(&b)[npages], std::vector<u8>& r, unsigned granu>
static void SetPages(unsigned size, unsigned baseaddr, unsigned index)
{
    for(unsigned v = r.size() + index * size,
                 p = baseaddr / granu;
                 p < (baseaddr + size) / granu && p < npages;
                 ++p, v += granu)
        b[p] = &r[v % r.size()];
} 

uint_least8_t& ROM::chrAccess(const uint_least16_t &address) {
    return pimpl->chr.at(address);
}

void ROM::init() {        
    
    //auto& SetVROM = SetPages<VROM_Pages,Vbanks,VRAM,VROM_Granularity>;
    //auto& SetROM  = SetPages< ROM_Pages, banks, ROM, ROM_Granularity>;

    //SetPages<VROM_Pages,Vbanks,pimpl->chr,VROM_Granularity>(0x2000, 0x0000, 0);
    /*
    for(unsigned v=0; v<4; ++v) {
        SetROM(0x4000, v*0x4000, v==3 ? -1 : 0);
    }*/
}