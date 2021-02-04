#pragma once

#include <functional>

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

    void step();

private:

    Register A, B, C, D, E, H, L;
    Pointer SP, PC;
    Flag F;
    MMU* p_mmu;

    Byte opcode;
    Byte arg1, arg2;
    std::function< void() > opcode_lookup[256];

private:

    void fetch();
    void decode();
    void execute();
    Pointer combine( Register high, Register low);
};