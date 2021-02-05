#include "MMU.h"

MMU::MMU(){}

Byte MMU::read( Pointer address )
{
    if( address >= 0xc000 && address <= 0xdfff ){
        return internal_ram[address - SEGMENT_INTERNAL_RAM.start];
    }
    return 0;
}

void MMU::write( Pointer address, Byte value)
{
    if( address >= 0xc000 && address <= 0xdfff ){
        internal_ram[address - SEGMENT_INTERNAL_RAM.start] = value;
    }
}
