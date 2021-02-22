#pragma once

#include "CPU.h"
#include "MMU.h"
#include "Screen.h"

class Gameboy{

public:

    Gameboy();
    void start();

private:

    MMU mmu;
    CPU cpu;
    Screen screen;
};