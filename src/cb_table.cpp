#include "CPU.h"
#include <cmath>

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
    Byte temp = (r>>1)+(r&(0b10000000));
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

void CPU::BIT(int bit_ind, Register &r)
{
    if (r&((Byte)(pow(2,bit_ind))) == 0)
    {
        F.Z = 0;
    }
    F.N = 0;
    F.H = 1;
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

    cb_opcode_lookup[0x40] = [this](){ BIT(0,B) };
    cb_opcode_lookup[0x41] = [this](){ BIT(0,C) };
    cb_opcode_lookup[0x42] = [this](){ BIT(0,D) };
    cb_opcode_lookup[0x43] = [this](){ BIT(0,E) };
    cb_opcode_lookup[0x44] = [this](){ BIT(0,H) };
    cb_opcode_lookup[0x45] = [this](){ BIT(0,L) };
    cb_opcode_lookup[0x46] = [this](){};
    cb_opcode_lookup[0x47] = [this](){ BIT(0,A) };
    cb_opcode_lookup[0x48] = [this](){ BIT(1,B) };
    cb_opcode_lookup[0x49] = [this](){ BIT(1,C) };
    cb_opcode_lookup[0x4a] = [this](){ BIT(1,D) };
    cb_opcode_lookup[0x4b] = [this](){ BIT(1,E) };
    cb_opcode_lookup[0x4c] = [this](){ BIT(1,H) };
    cb_opcode_lookup[0x4d] = [this](){ BIT(1,L) };
    cb_opcode_lookup[0x4e] = [this](){};
    cb_opcode_lookup[0x4f] = [this](){ BIT(1,A) };

    cb_opcode_lookup[0x50] = [this](){ BIT(2,B) };
    cb_opcode_lookup[0x51] = [this](){ BIT(2,C) };
    cb_opcode_lookup[0x52] = [this](){ BIT(2,D) };
    cb_opcode_lookup[0x53] = [this](){ BIT(2,E) };
    cb_opcode_lookup[0x54] = [this](){ BIT(2,H) };
    cb_opcode_lookup[0x55] = [this](){ BIT(2,L) };
    cb_opcode_lookup[0x56] = [this](){};
    cb_opcode_lookup[0x57] = [this](){ BIT(2,A) };
    cb_opcode_lookup[0x58] = [this](){ BIT(3,B) };
    cb_opcode_lookup[0x59] = [this](){ BIT(3,C) };
    cb_opcode_lookup[0x5a] = [this](){ BIT(3,D) };
    cb_opcode_lookup[0x5b] = [this](){ BIT(3,E) };
    cb_opcode_lookup[0x5c] = [this](){ BIT(3,H) };
    cb_opcode_lookup[0x5d] = [this](){ BIT(3,L) };
    cb_opcode_lookup[0x5e] = [this](){};
    cb_opcode_lookup[0x5f] = [this](){ BIT(3,A) };

    cb_opcode_lookup[0x60] = [this](){ BIT(4,B) };
    cb_opcode_lookup[0x61] = [this](){ BIT(4,C) };
    cb_opcode_lookup[0x62] = [this](){ BIT(4,D) };
    cb_opcode_lookup[0x63] = [this](){ BIT(4,E) };
    cb_opcode_lookup[0x64] = [this](){ BIT(4,H) };
    cb_opcode_lookup[0x65] = [this](){ BIT(4,L) };
    cb_opcode_lookup[0x66] = [this](){};
    cb_opcode_lookup[0x67] = [this](){ BIT(4,A) };
    cb_opcode_lookup[0x68] = [this](){ BIT(5,B) };
    cb_opcode_lookup[0x69] = [this](){ BIT(5,C) };
    cb_opcode_lookup[0x6a] = [this](){ BIT(5,D) };
    cb_opcode_lookup[0x6b] = [this](){ BIT(5,E) };
    cb_opcode_lookup[0x6c] = [this](){ BIT(5,H) };
    cb_opcode_lookup[0x6d] = [this](){ BIT(5,L) };
    cb_opcode_lookup[0x6e] = [this](){};
    cb_opcode_lookup[0x6f] = [this](){ BIT(5,A) };

    cb_opcode_lookup[0x70] = [this](){ BIT(6,B) };
    cb_opcode_lookup[0x71] = [this](){ BIT(6,C) };
    cb_opcode_lookup[0x72] = [this](){ BIT(6,D) };
    cb_opcode_lookup[0x73] = [this](){ BIT(6,E) };
    cb_opcode_lookup[0x74] = [this](){ BIT(6,H) };
    cb_opcode_lookup[0x75] = [this](){ BIT(6,L) };
    cb_opcode_lookup[0x76] = [this](){};
    cb_opcode_lookup[0x77] = [this](){ BIT(7,A) };
    cb_opcode_lookup[0x78] = [this](){ BIT(7,B) };
    cb_opcode_lookup[0x79] = [this](){ BIT(7,C) };
    cb_opcode_lookup[0x7a] = [this](){ BIT(7,D) };
    cb_opcode_lookup[0x7b] = [this](){ BIT(7,E) };
    cb_opcode_lookup[0x7c] = [this](){ BIT(7,H) };
    cb_opcode_lookup[0x7d] = [this](){ BIT(7,L) };
    cb_opcode_lookup[0x7e] = [this](){};
    cb_opcode_lookup[0x7f] = [this](){ BIT(7,A) };

    cb_opcode_lookup[0x80] = [this](){};
    cb_opcode_lookup[0x81] = [this](){};
    cb_opcode_lookup[0x82] = [this](){};
    cb_opcode_lookup[0x83] = [this](){};
    cb_opcode_lookup[0x84] = [this](){};
    cb_opcode_lookup[0x85] = [this](){};
    cb_opcode_lookup[0x86] = [this](){};
    cb_opcode_lookup[0x87] = [this](){};
    cb_opcode_lookup[0x88] = [this](){};
    cb_opcode_lookup[0x89] = [this](){};
    cb_opcode_lookup[0x8a] = [this](){};
    cb_opcode_lookup[0x8b] = [this](){};
    cb_opcode_lookup[0x8c] = [this](){};
    cb_opcode_lookup[0x8d] = [this](){};
    cb_opcode_lookup[0x8e] = [this](){};
    cb_opcode_lookup[0x8f] = [this](){};

    cb_opcode_lookup[0x90] = [this](){};
    cb_opcode_lookup[0x91] = [this](){};
    cb_opcode_lookup[0x92] = [this](){};
    cb_opcode_lookup[0x93] = [this](){};
    cb_opcode_lookup[0x94] = [this](){};
    cb_opcode_lookup[0x95] = [this](){};
    cb_opcode_lookup[0x96] = [this](){};
    cb_opcode_lookup[0x97] = [this](){};
    cb_opcode_lookup[0x98] = [this](){};
    cb_opcode_lookup[0x99] = [this](){};
    cb_opcode_lookup[0x9a] = [this](){};
    cb_opcode_lookup[0x9b] = [this](){};
    cb_opcode_lookup[0x9c] = [this](){};
    cb_opcode_lookup[0x9d] = [this](){};
    cb_opcode_lookup[0x9e] = [this](){};
    cb_opcode_lookup[0x9f] = [this](){};

}
