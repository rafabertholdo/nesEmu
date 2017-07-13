#ifndef ADDRESSING_H
#define ADDRESSING_H

#include "Utils.h"

enum AddressingMode {     
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
    indirectY,
    accumulator
};

#endif
