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

void CPU::RL(Register &r)
{
    bool temp1 = r>>7;
    Byte temp2 = r<<1;
    r = temp2 + F.C;
    F.C = temp1;
    if (r==0)
    {
        F.Z = 1;
    }
    F.N = 0;
    F.H = 0;
}

void CPU::RR(Register &r)
{
    bool temp1 = r<<7;
    Byte temp2 = r>>1;
    r = temp2+(F.C<<7);
    F.C = temp1;
    if (r==0)
    {
        F.Z = 1;
    }
    F.N = 0;
    F.H = 0;
}

void CPU::SLA(Register &r)
{
    F.C = r>>7;
    Byte temp = r<<1;
    r = temp;
    if (r==0)
    {
        F.Z = 1;
    }
    F.N = 0;
    F.H = 0;
}

void CPU::SRA(Register &r)
{
    F.C = r<<7;
    Byte temp = (r>>1)+(r&128);
    r = temp;
    if (r==0)
    {
        F.Z = 1;
    }
    F.N = 0;
    F.H = 0;
}

void CPU::SRL(Register &r)
{
    F.C = r<<7;
    Byte temp = r>>1;
    r = temp;
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

    cb_opcode_lookup[0x10] = [this](){ RL(B); };
    cb_opcode_lookup[0x11] = [this](){ RL(C); };
    cb_opcode_lookup[0x12] = [this](){ RL(D); };
    cb_opcode_lookup[0x13] = [this](){ RL(E); };
    cb_opcode_lookup[0x14] = [this](){ RL(H); };
    cb_opcode_lookup[0x15] = [this](){ RL(L); };
    cb_opcode_lookup[0x16] = [this](){};
    cb_opcode_lookup[0x17] = [this](){ RL(A); };
    cb_opcode_lookup[0x18] = [this](){ RR(B); };
    cb_opcode_lookup[0x19] = [this](){ RR(C); };
    cb_opcode_lookup[0x1a] = [this](){ RR(D); };
    cb_opcode_lookup[0x1b] = [this](){ RR(E); };
    cb_opcode_lookup[0x1c] = [this](){ RR(H); };
    cb_opcode_lookup[0x1d] = [this](){ RR(L); };
    cb_opcode_lookup[0x1e] = [this](){};
    cb_opcode_lookup[0x1f] = [this](){ RR(A); };

    cb_opcode_lookup[0x20] = [this](){ SLA(B) };
    cb_opcode_lookup[0x21] = [this](){ SLA(C) };
    cb_opcode_lookup[0x22] = [this](){ SLA(D) };
    cb_opcode_lookup[0x23] = [this](){ SLA(E) };
    cb_opcode_lookup[0x24] = [this](){ SLA(H) };
    cb_opcode_lookup[0x25] = [this](){ SLA(L) };
    cb_opcode_lookup[0x26] = [this](){};
    cb_opcode_lookup[0x27] = [this](){ SLA(A) };
    cb_opcode_lookup[0x28] = [this](){ SRA(B) };
    cb_opcode_lookup[0x29] = [this](){ SRA(C) };
    cb_opcode_lookup[0x2a] = [this](){ SRA(D) };
    cb_opcode_lookup[0x2b] = [this](){ SRA(E) };
    cb_opcode_lookup[0x2c] = [this](){ SRA(H) };
    cb_opcode_lookup[0x2d] = [this](){ SRA(L) };
    cb_opcode_lookup[0x2e] = [this](){};
    cb_opcode_lookup[0x2f] = [this](){ SRA(A) };

    cb_opcode_lookup[0x30] = [this](){};
    cb_opcode_lookup[0x31] = [this](){};
    cb_opcode_lookup[0x32] = [this](){};
    cb_opcode_lookup[0x33] = [this](){};
    cb_opcode_lookup[0x34] = [this](){};
    cb_opcode_lookup[0x35] = [this](){};
    cb_opcode_lookup[0x36] = [this](){};
    cb_opcode_lookup[0x37] = [this](){};
    cb_opcode_lookup[0x38] = [this](){ SRL(B) };
    cb_opcode_lookup[0x39] = [this](){ SRL(C) };
    cb_opcode_lookup[0x3a] = [this](){ SRL(D) };
    cb_opcode_lookup[0x3b] = [this](){ SRL(E) };
    cb_opcode_lookup[0x3c] = [this](){ SRL(H) };
    cb_opcode_lookup[0x3d] = [this](){ SRL(L) };
    cb_opcode_lookup[0x3e] = [this](){};
    cb_opcode_lookup[0x3f] = [this](){ SRL(A) };
}
