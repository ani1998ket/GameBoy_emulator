#include "Gameboy.h"

Gameboy::Gameboy()
{
    cpu.connect(&mmu);
    cpu.print_state();
}