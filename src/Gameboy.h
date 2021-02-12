#pragma once

#include "CPU.h"
#include "MMU.h"

class Gameboy{

public:

    Gameboy();

private:

    MMU mmu;
    CPU cpu;
    
};