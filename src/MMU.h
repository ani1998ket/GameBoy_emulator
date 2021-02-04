#pragma once

#include "utils.h"

static const int INTERNAL_RAM_SIZE = 8192;

class MMU{

public:

    MMU();

    Byte read( Pointer address );
    void write( Pointer address, Byte value);

private:

    Byte internal_ram[INTERNAL_RAM_SIZE] = {0};

};