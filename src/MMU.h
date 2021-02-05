#pragma once

#include "utils.h"

struct MemorySegment{
    Pointer start;
    Pointer end;
    constexpr Pointer size() const { return (end-start)+1; }
};

const struct MemorySegment SEGMENT_INTERNAL_RAM{0xc000,0xdfff};

class MMU{

public:

    MMU();

    Byte read( Pointer address );
    void write( Pointer address, Byte value);

private:

    Byte internal_ram[ SEGMENT_INTERNAL_RAM.size() ] = {0};

};