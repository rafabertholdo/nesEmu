#ifndef ROM_H
#define ROM_H

#include <string>
#include <memory>
#include <vector>
#include "Utils.h"

using namespace std;

// An unsigned char can store 1 Bytes (8bits) of data (0-255)
static const u8 HEADER_SIZE = 16;
static const unsigned CHR_Granularity = 0x0400, CHR_Pages = 0x2000  / CHR_Granularity;
static const unsigned PRG_Granularity = 0x2000, PRG_Pages = 0x10000 / PRG_Granularity;    
static const unsigned PRG_Start = 0x8000;

class ROM {        
    class impl;
    unique_ptr<impl> pimpl;
public:  
    unsigned char CIRAM[0x800];
    u32 prgMap[PRG_Pages];
    u32 chrMap[CHR_Pages];

    void map_prg(int pageKBs, int slot, int bank);
    void map_chr(int pageKBs, int slot, int bank);

    unsigned char *nameTable[4] = { CIRAM+0x0000, CIRAM+0x0400, CIRAM+0x0000, CIRAM+0x0400 }; //Vertical mirroring: $2000 equals $2800 and $2400 equals $2C00

    ROM(const char* filePath);
    ~ROM();
    uint_least8_t prgAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);    
    uint_least8_t& chrAccess(const uint_least16_t &address);
    void init();
};

#endif