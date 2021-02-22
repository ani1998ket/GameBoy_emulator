#include "Gameboy.h"

Gameboy::Gameboy()
{
    cpu.connect(&mmu);
    screen.connect(&mmu);
    for( int i = 0; i < 160*144; i++ ){
        mmu.write( SEGMENT_VIDEO_RAM.start + i , 0b00011011 );
    }
}

void Gameboy::start(){
    while( screen.is_open() ){
        screen.loop();
    }
}