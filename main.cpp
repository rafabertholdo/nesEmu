#include <fstream>
#include <iostream>
#include "Rom.h"
#include "CPU.h"
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  //if ( argc != 2 ) {
//    cout << "usage: " << argv[0] << " <filename>\n";
  //} else {
    Rom cartige("mario.nes");
    CPU cpu;
    cpu.loadRom(cartige);
    cpu.run();    
  //}
}



