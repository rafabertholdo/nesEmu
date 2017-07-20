#ifndef ROM_H
#define ROM_H

#include <string>
#include <memory>
#include <vector>
#include "utils.h"

using namespace std;

// An unsigned char can store 1 Bytes (8bits) of data (0-255)
static const u8 HEADER_SIZE = 16;
static const unsigned CHR_Granularity = 0x0400, CHR_Pages = 0x2000  / CHR_Granularity;
static const unsigned ROM_Granularity = 0x2000, ROM_Pages = 0x10000 / ROM_Granularity;    

class ROM {        
    class impl;
    unique_ptr<impl> pimpl;
public:  
    unsigned char NRAM[0x1000], PRAM[0x2000];
    unsigned char* banks[ROM_Pages]  = {};
    unsigned char* Vbanks[CHR_Pages] = {};
    unsigned char *Nta[4] = { NRAM+0x0000, NRAM+0x0400, NRAM+0x0000, NRAM+0x0400 };

    ROM(const char* filePath);
    ~ROM();
    uint_least8_t prgAccess(const uint_least16_t &address, const uint_least8_t &value, const bool &write);    
    uint_least8_t& chrAccess(const uint_least16_t &address);
    void init();
};

#endif