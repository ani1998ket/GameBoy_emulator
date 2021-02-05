#include "MMU.h"

MMU::MMU(){}

Byte MMU::read( Pointer address )
{
    if( address >= SEGMENT_INTERNAL_RAM.start && address <= SEGMENT_INTERNAL_RAM.end){
        return internal_ram[ address - SEGMENT_INTERNAL_RAM.start ];
    }
    else 
    if( address >= SEGMENT_ECHO_INTERNAL_RAM.start && address <= SEGMENT_ECHO_INTERNAL_RAM.end ){
        return internal_ram[ address - SEGMENT_ECHO_INTERNAL_RAM.start ];
    }
    return 0;
}

void MMU::write( Pointer address, Byte value )
{
    if( address >= SEGMENT_INTERNAL_RAM.start && address <= SEGMENT_INTERNAL_RAM.end){
        internal_ram[ address - SEGMENT_INTERNAL_RAM.start ] = value;
    }
    else 
    if( address >= SEGMENT_ECHO_INTERNAL_RAM.start && address <= SEGMENT_ECHO_INTERNAL_RAM.end ){
        internal_ram[ address - SEGMENT_ECHO_INTERNAL_RAM.start ] = value;
    }
}
