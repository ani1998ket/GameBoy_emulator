#pragma once

#include "utils.h"
#include "MMU.h"

struct Flag{
    bool Z, N, H, C;
};

class CPU{
public:
    CPU();
    void connect(MMU* p_mmu);
    void reset();

private:

    Register A, B, C, D, E, H, L;
    Pointer SP, PC;
    Flag F;
    MMU* p_mmu;

    Byte opcode;
    Byte arg1, arg2;

private:

    Pointer combine( Register high, Register low);
};