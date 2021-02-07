#include "CPU.h"

#define BC combine(B, C)
#define DE combine(D, E)
#define HL combine(H, L)

void CPU::RLC(Register &r)
{
    F.C = r>>7;
    Byte temp = r<<1;
    r = temp + F.C;
    if (r == 0)
    {
        F.Z = 1;
    }
    F.N = 0;
    F.H = 0;
}

void CPU::RRC(Register &r)
{
    Byte temp1 = r<<7;
    Byte temp2 = r>>1;
    r = temp1 + temp2;
    if (r == 0)
    {
        F.Z = 1;
    }
    if (temp1>0)
    {
        F.C = 1;
    }
    else
    {
        F.C = 0;
    }
    F.N = 0;
    F.H = 0;
}
void CPU::init_cb(){
    cb_opcode_lookup[0x00] = [this](){ RLC(B); };
    cb_opcode_lookup[0x01] = [this](){ RLC(C); };
    cb_opcode_lookup[0x02] = [this](){ RLC(D); };
    cb_opcode_lookup[0x03] = [this](){ RLC(E); };
    cb_opcode_lookup[0x04] = [this](){ RLC(H); };
    cb_opcode_lookup[0x05] = [this](){ RLC(L); };
    cb_opcode_lookup[0x06] = [this](){};
    cb_opcode_lookup[0x07] = [this](){ RLC(A); };
    cb_opcode_lookup[0x08] = [this](){ RRC(B); };
    cb_opcode_lookup[0x09] = [this](){ RRC(C); };
    cb_opcode_lookup[0x0a] = [this](){ RRC(D); };
    cb_opcode_lookup[0x0b] = [this](){ RRC(E); };
    cb_opcode_lookup[0x0c] = [this](){ RRC(H); };
    cb_opcode_lookup[0x0d] = [this](){ RRC(L); };
    cb_opcode_lookup[0x0e] = [this](){};
    cb_opcode_lookup[0x0f] = [this](){ RRC(A); };

    cb_opcode_lookup[0x10] = [this](){};
    cb_opcode_lookup[0x11] = [this](){};
    cb_opcode_lookup[0x12] = [this](){};
    cb_opcode_lookup[0x13] = [this](){};
    cb_opcode_lookup[0x14] = [this](){};
    cb_opcode_lookup[0x15] = [this](){};
    cb_opcode_lookup[0x16] = [this](){};
    cb_opcode_lookup[0x17] = [this](){};
    cb_opcode_lookup[0x18] = [this](){};
    cb_opcode_lookup[0x19] = [this](){};
    cb_opcode_lookup[0x1a] = [this](){};
    cb_opcode_lookup[0x1b] = [this](){};
    cb_opcode_lookup[0x1c] = [this](){};
    cb_opcode_lookup[0x1d] = [this](){};
    cb_opcode_lookup[0x1e] = [this](){};
    cb_opcode_lookup[0x1f] = [this](){};

    cb_opcode_lookup[0x20] = [this](){};
    cb_opcode_lookup[0x21] = [this](){};
    cb_opcode_lookup[0x22] = [this](){};
    cb_opcode_lookup[0x23] = [this](){};
    cb_opcode_lookup[0x24] = [this](){};
    cb_opcode_lookup[0x25] = [this](){};
    cb_opcode_lookup[0x26] = [this](){};
    cb_opcode_lookup[0x27] = [this](){};
    cb_opcode_lookup[0x28] = [this](){};
    cb_opcode_lookup[0x29] = [this](){};
    cb_opcode_lookup[0x2a] = [this](){};
    cb_opcode_lookup[0x2b] = [this](){};
    cb_opcode_lookup[0x2c] = [this](){};
    cb_opcode_lookup[0x2d] = [this](){};
    cb_opcode_lookup[0x2e] = [this](){};
    cb_opcode_lookup[0x2f] = [this](){};

    cb_opcode_lookup[0x30] = [this](){};
    cb_opcode_lookup[0x31] = [this](){};
    cb_opcode_lookup[0x32] = [this](){};
    cb_opcode_lookup[0x33] = [this](){};
    cb_opcode_lookup[0x34] = [this](){};
    cb_opcode_lookup[0x35] = [this](){};
    cb_opcode_lookup[0x36] = [this](){};
    cb_opcode_lookup[0x37] = [this](){};
    cb_opcode_lookup[0x38] = [this](){};
    cb_opcode_lookup[0x39] = [this](){};
    cb_opcode_lookup[0x3a] = [this](){};
    cb_opcode_lookup[0x3b] = [this](){};
    cb_opcode_lookup[0x3c] = [this](){};
    cb_opcode_lookup[0x3d] = [this](){};
    cb_opcode_lookup[0x3e] = [this](){};
    cb_opcode_lookup[0x3f] = [this](){};
}
