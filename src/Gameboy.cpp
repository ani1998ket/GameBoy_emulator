#include "Gameboy.h"

Gameboy::Gameboy()
{
    cpu.connect(&mmu);
}