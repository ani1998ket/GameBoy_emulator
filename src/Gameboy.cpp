#include "Gameboy.h"

Gameboy::Gameboy()
{
    cpu.connect(&mmu);
    screen.connect(&mmu);
}

void Gameboy::start(){
    while( screen.is_open() ){
        screen.render();
    }
}