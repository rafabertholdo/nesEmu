#ifndef ADDRESSING_H
#define ADDRESSING_H

#include "Utils.h"

enum AddressingMode { 
    noAddressing,
    immediate,
    absolute, 
    absoluteX,
    absoluteY,
    zeroPage,
    zeroPageX,
    zeroPageY,
    relative,
    implict,
    indirect,
    indirectX,
    indirectY
};

class Addressing {
public:
    u8 dataLength = 0;

    u8 getInstructionLength() {
        return dataLength + 1;
    }
    
    virtual vector<u8> getData(vector<u8> instructionData) = 0;
};

class NoAddressing : public Addressing {
public:
    u8 dataLength = 0;
    vector<u8> getData(vector<u8> instructionData) override {
        return vector<u8>();
    };
};


class ImmediateAddressing : public Addressing {
public:
    u8 dataLength = 1;
    /*
    u8 getData(const vector<u8> &data){
        return data[0];
    }*/
};

#endif
