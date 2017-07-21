#include <fstream>
#include <iostream>
#include "Rom.h"
#include "CPU.h"
#include "PPU.h"
#include "IO.h"
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {

  if ( argc != 2 ) {
    cout << "usage: " << argv[0] << " <filename>\n";
  } else {    
    auto io = make_shared<IO>();
    auto rom = make_shared<ROM>(argv[1]);
    auto cpu = make_shared<CPU>(io);
    auto ppu = make_shared<PPU>(io,cpu,rom);
    cpu->setPPU(ppu);
    cpu->loadRom(rom);
    cpu->run();
  }  
}



