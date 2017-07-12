#ifndef UTILS_H
#define UTILS_H

#include <vector>

// Integer types
typedef uint_least32_t u32;
typedef uint_least16_t u16;
typedef uint_least8_t   u8;
typedef  int_least8_t   s8;

template<typename T>
struct Utils {
    static void printVector(const std::vector<T> &v);
    static void printHex(const T &hexValue);
    static uint_least16_t getLittleEndianValue(const std::vector<T> &data);
};


#endif