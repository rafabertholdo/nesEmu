#ifndef ROM_H
#define ROM_H

#include <string>
#include <memory>
#include <vector>

using namespace std;

// An unsigned char can store 1 Bytes (8bits) of data (0-255)
typedef uint_least8_t BYTE;
static const BYTE HEADER_SIZE = 16;

class Rom {        
    class impl;
    unique_ptr<impl> pimpl;
public:  
    Rom(const char* filePath);
    ~Rom();
    uint_least8_t access(const uint_least16_t &address, const uint_least8_t &value, const bool &write);
};

#endif