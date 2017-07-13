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

#endif
