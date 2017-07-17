#include "Utils.h"
#include <iostream>
#include <iomanip>

template<typename T>
void Utils<T>::printVector(const std::vector<T> &v) {
    for(unsigned i = 0; i< v.size(); ++i) {
        printHex(v[i]);
        std::cout << " ";
        if ((i + 1) % 8 == 0) {
            std::cout << std::endl;        
        }
    }
    std::cout << std::endl;
}

template<typename T>
T Utils<T>::printHex(const T &hexValue) {
	T numberOfCharacters = hexValue > 0xFF ? sizeof(T) * 2 : 2;
    std::cout << std::setw(numberOfCharacters) << std::setfill('0') << std::uppercase << std::hex << static_cast<int>(hexValue);
	return numberOfCharacters;
}

template<typename T>
uint_least16_t Utils<T>::getLittleEndianValue(const std::vector<T> &data) {
    uint_least16_t value = 0;
    for (int i = 0; i < data.size(); i++) {
        value |= data[i] << (i * 8);
    }
    return value;
}

template<typename T>
void Utils<T>::appendVector(std::vector<T> &destination, const std::vector<T> &source) {
    destination.reserve(destination.size()+source.size()); // Reserve space first
    destination.insert(destination.end(),source.begin(),source.end());
}
