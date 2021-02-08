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

void CPU::RLC_16(Pointer &r)
{
    F.C = r>>15;
    Pointer temp = r<<1;
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

void CPU::RRC_16(Pointer &r)
{
    Pointer temp1 = r<<15;
    Pointer temp2 = r>>1;
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

void CPU::RL_16(Pointer &r)
{
    bool temp1 = r>>15;
    Pointer temp2 = r<<1;
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
void CPU::RR_16(Pointer &r)
{
    bool temp1 = r<<15;
    Pointer temp2 = r>>1;
    r = temp2+(F.C<<15);
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

void CPU::RES(int bit_ind, Register &r)
{
    bool bit[8];
    for (int i = 0; i < 8; i++)
    {
        bit[i] = r&((Byte)(pow(2,i)));
    }
    bit[bit_ind] = 0;
    Byte temp = 0;
    for (int i = 0; i < 8; i++)
    {
        if (bit[i]==1)
        {
            temp = temp+(Byte)(pow(2,i));
        }
    }
    r = temp;
}

void CPU::SET(int bit_ind, Register &r)
{
    bool bit[8];
    for (int i = 0; i < 8; i++)
    {
        bit[i] = r&((Byte)(pow(2,i)));
    }
    bit[bit_ind] = 1;
    Byte temp = 0;
    for (int i = 0; i < 8; i++)
    {
        if (bit[i]==1)
        {
            temp = temp+(Byte)(pow(2,i));
        }
    }
    r = temp;
}
void CPU::SWAP(Register &r)
{
    Byte temp1 = (r&0b11110000)>>4;
    Byte temp2 = (r&0b00001111)<<4;
    r = temp1+temp2;
}
void CPU::init_cb(){
    cb_opcode_lookup[0x00] = [this](){ RLC(B); };
    cb_opcode_lookup[0x01] = [this](){ RLC(C); };
    cb_opcode_lookup[0x02] = [this](){ RLC(D); };
    cb_opcode_lookup[0x03] = [this](){ RLC(E); };
    cb_opcode_lookup[0x04] = [this](){ RLC(H); };
    cb_opcode_lookup[0x05] = [this](){ RLC(L); };
    cb_opcode_lookup[0x06] = [this](){ RLC_16(HL); };
    cb_opcode_lookup[0x07] = [this](){ RLC(A); };
    cb_opcode_lookup[0x08] = [this](){ RRC(B); };
    cb_opcode_lookup[0x09] = [this](){ RRC(C); };
    cb_opcode_lookup[0x0a] = [this](){ RRC(D); };
    cb_opcode_lookup[0x0b] = [this](){ RRC(E); };
    cb_opcode_lookup[0x0c] = [this](){ RRC(H); };
    cb_opcode_lookup[0x0d] = [this](){ RRC(L); };
    cb_opcode_lookup[0x0e] = [this](){ RRC_16(HL); };
    cb_opcode_lookup[0x0f] = [this](){ RRC(A); };

    cb_opcode_lookup[0x10] = [this](){ RL(B); };
    cb_opcode_lookup[0x11] = [this](){ RL(C); };
    cb_opcode_lookup[0x12] = [this](){ RL(D); };
    cb_opcode_lookup[0x13] = [this](){ RL(E); };
    cb_opcode_lookup[0x14] = [this](){ RL(H); };
    cb_opcode_lookup[0x15] = [this](){ RL(L); };
    cb_opcode_lookup[0x16] = [this](){ RL_16(HL) };
    cb_opcode_lookup[0x17] = [this](){ RL(A); };
    cb_opcode_lookup[0x18] = [this](){ RR(B); };
    cb_opcode_lookup[0x19] = [this](){ RR(C); };
    cb_opcode_lookup[0x1a] = [this](){ RR(D); };
    cb_opcode_lookup[0x1b] = [this](){ RR(E); };
    cb_opcode_lookup[0x1c] = [this](){ RR(H); };
    cb_opcode_lookup[0x1d] = [this](){ RR(L); };
    cb_opcode_lookup[0x1e] = [this](){ RR_16(HL) };
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

    cb_opcode_lookup[0x30] = [this](){ SWAP(B) };
    cb_opcode_lookup[0x31] = [this](){ SWAP(C) };
    cb_opcode_lookup[0x32] = [this](){ SWAP(D) };
    cb_opcode_lookup[0x33] = [this](){ SWAP(E) };
    cb_opcode_lookup[0x34] = [this](){ SWAP(H) };
    cb_opcode_lookup[0x35] = [this](){ SWAP(L) };
    cb_opcode_lookup[0x36] = [this](){};
    cb_opcode_lookup[0x37] = [this](){ SWAP(A) };
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

    cb_opcode_lookup[0x80] = [this](){ RES(0,B) };
    cb_opcode_lookup[0x81] = [this](){ RES(0,C) };
    cb_opcode_lookup[0x82] = [this](){ RES(0,D) };
    cb_opcode_lookup[0x83] = [this](){ RES(0,E) };
    cb_opcode_lookup[0x84] = [this](){ RES(0,H) };
    cb_opcode_lookup[0x85] = [this](){ RES(0,L) };
    cb_opcode_lookup[0x86] = [this](){};
    cb_opcode_lookup[0x87] = [this](){ RES(0,A) };
    cb_opcode_lookup[0x88] = [this](){ RES(1,B) };
    cb_opcode_lookup[0x89] = [this](){ RES(1,C) };
    cb_opcode_lookup[0x8a] = [this](){ RES(1,D) };
    cb_opcode_lookup[0x8b] = [this](){ RES(1,E) };
    cb_opcode_lookup[0x8c] = [this](){ RES(1,H) };
    cb_opcode_lookup[0x8d] = [this](){ RES(1,L) } ;
    cb_opcode_lookup[0x8e] = [this](){};
    cb_opcode_lookup[0x8f] = [this](){ RES(1,A) };

    cb_opcode_lookup[0x90] = [this](){ RES(2,B) };
    cb_opcode_lookup[0x91] = [this](){ RES(2,C) };
    cb_opcode_lookup[0x92] = [this](){ RES(2,D) };
    cb_opcode_lookup[0x93] = [this](){ RES(2,E) };
    cb_opcode_lookup[0x94] = [this](){ RES(2,H) };
    cb_opcode_lookup[0x95] = [this](){ RES(2,L) };
    cb_opcode_lookup[0x96] = [this](){};
    cb_opcode_lookup[0x97] = [this](){ RES(2,A) };
    cb_opcode_lookup[0x98] = [this](){ RES(3,B) };
    cb_opcode_lookup[0x99] = [this](){ RES(3,C) };
    cb_opcode_lookup[0x9a] = [this](){ RES(3,D) };
    cb_opcode_lookup[0x9b] = [this](){ RES(3,E) };
    cb_opcode_lookup[0x9c] = [this](){ RES(3,H) };
    cb_opcode_lookup[0x9d] = [this](){ RES(3,L) };
    cb_opcode_lookup[0x9e] = [this](){};
    cb_opcode_lookup[0x9f] = [this](){ RES(3,A) };

    cb_opcode_lookup[0xa0] = [this](){ RES(4,B) };
    cb_opcode_lookup[0xa1] = [this](){ RES(4,C) };
    cb_opcode_lookup[0xa2] = [this](){ RES(4,D) };
    cb_opcode_lookup[0xa3] = [this](){ RES(4,E) };
    cb_opcode_lookup[0xa4] = [this](){ RES(4,H) };
    cb_opcode_lookup[0xa5] = [this](){ RES(4,L) };
    cb_opcode_lookup[0xa6] = [this](){};
    cb_opcode_lookup[0xa7] = [this](){ RES(4,A) };
    cb_opcode_lookup[0xa8] = [this](){ RES(5,B) };
    cb_opcode_lookup[0xa9] = [this](){ RES(5,C) };
    cb_opcode_lookup[0xaa] = [this](){ RES(5D) };
    cb_opcode_lookup[0xab] = [this](){ RES(5,E) };
    cb_opcode_lookup[0xac] = [this](){ RES(5,H) };
    cb_opcode_lookup[0xad] = [this](){ RES(5,L) };
    cb_opcode_lookup[0xae] = [this](){};
    cb_opcode_lookup[0xaf] = [this](){ RES(5,A) };

    cb_opcode_lookup[0xb0] = [this](){ RES(6,B) };
    cb_opcode_lookup[0xb1] = [this](){ RES(6,C) };
    cb_opcode_lookup[0xb2] = [this](){ RES(6,D) };
    cb_opcode_lookup[0xb3] = [this](){ RES(6,E) };
    cb_opcode_lookup[0xb4] = [this](){ RES(6,H) };
    cb_opcode_lookup[0xb5] = [this](){ RES(6,L) };
    cb_opcode_lookup[0xb6] = [this](){};
    cb_opcode_lookup[0xb7] = [this](){ RES(6,A) };
    cb_opcode_lookup[0xb8] = [this](){ RES(7,B) };
    cb_opcode_lookup[0xb9] = [this](){ RES(7,C) };
    cb_opcode_lookup[0xba] = [this](){ RES(7,D) };
    cb_opcode_lookup[0xbb] = [this](){ RES(7,E) };
    cb_opcode_lookup[0xbc] = [this](){ RES(7,H) };
    cb_opcode_lookup[0xbd] = [this](){ RES(7,L) };
    cb_opcode_lookup[0xbe] = [this](){};
    cb_opcode_lookup[0xbf] = [this](){ RES(7,A) };

    cb_opcode_lookup[0xc0] = [this](){ SET(0,B) };
    cb_opcode_lookup[0xc1] = [this](){ SET(0,C) };
    cb_opcode_lookup[0xc2] = [this](){ SET(0,D) };
    cb_opcode_lookup[0xc3] = [this](){ SET(0,E) };
    cb_opcode_lookup[0xc4] = [this](){ SET(0,H) };
    cb_opcode_lookup[0xc5] = [this](){ SET(0,L) };
    cb_opcode_lookup[0xc6] = [this](){};
    cb_opcode_lookup[0xc7] = [this](){ SET(0,A) };
    cb_opcode_lookup[0xc8] = [this](){ SET(1,B) };
    cb_opcode_lookup[0xc9] = [this](){ SET(1,C) };
    cb_opcode_lookup[0xca] = [this](){ SET(1,D) };
    cb_opcode_lookup[0xcb] = [this](){ SET(1,E) };
    cb_opcode_lookup[0xcc] = [this](){ SET(1,H) };
    cb_opcode_lookup[0xcd] = [this](){ SET(1,L) };
    cb_opcode_lookup[0xce] = [this](){};
    cb_opcode_lookup[0xcf] = [this](){ SET(1,A) };

    cb_opcode_lookup[0xd0] = [this](){ SET(2,B) };
    cb_opcode_lookup[0xd1] = [this](){ SET(2,C) };
    cb_opcode_lookup[0xd2] = [this](){ SET(2,D) };
    cb_opcode_lookup[0xd3] = [this](){ SET(2,E) };
    cb_opcode_lookup[0xd4] = [this](){ SET(2,H) };
    cb_opcode_lookup[0xd5] = [this](){ SET(2,L) };
    cb_opcode_lookup[0xd6] = [this](){};
    cb_opcode_lookup[0xd7] = [this](){ SET(2,A) };
    cb_opcode_lookup[0xd8] = [this](){ SET(3,B) };
    cb_opcode_lookup[0xd9] = [this](){ SET(3,C) };
    cb_opcode_lookup[0xda] = [this](){ SET(3,D) };
    cb_opcode_lookup[0xdb] = [this](){ SET(3,E) };
    cb_opcode_lookup[0xdc] = [this](){ SET(3,H) };
    cb_opcode_lookup[0xdd] = [this](){ SET(3,L) };
    cb_opcode_lookup[0xde] = [this](){};
    cb_opcode_lookup[0xdf] = [this](){ SET(3,A) };

    cb_opcode_lookup[0xe0] = [this](){ SET(4,B) };
    cb_opcode_lookup[0xe1] = [this](){ SET(4,C) };
    cb_opcode_lookup[0xe2] = [this](){ SET(4,D) };
    cb_opcode_lookup[0xe3] = [this](){ SET(4,E) };
    cb_opcode_lookup[0xe4] = [this](){ SET(4,H) };
    cb_opcode_lookup[0xe5] = [this](){ SET(4,L) };
    cb_opcode_lookup[0xe6] = [this](){};
    cb_opcode_lookup[0xe7] = [this](){ SET(4,A) };
    cb_opcode_lookup[0xe8] = [this](){ SET(5,B) };
    cb_opcode_lookup[0xe9] = [this](){ SET(5,C) };
    cb_opcode_lookup[0xea] = [this](){ SET(5,D) };
    cb_opcode_lookup[0xeb] = [this](){ SET(5,E) };
    cb_opcode_lookup[0xec] = [this](){ SET(5,H) };
    cb_opcode_lookup[0xed] = [this](){ SET(5,L) };
    cb_opcode_lookup[0xee] = [this](){};
    cb_opcode_lookup[0xef] = [this](){ SET(5,A) };

    cb_opcode_lookup[0xf0] = [this](){ SET(6,B) };
    cb_opcode_lookup[0xf1] = [this](){ SET(6,C) };
    cb_opcode_lookup[0xf2] = [this](){ SET(6,D) };
    cb_opcode_lookup[0xf3] = [this](){ SET(6,E) };
    cb_opcode_lookup[0xf4] = [this](){ SET(6,H) };
    cb_opcode_lookup[0xf5] = [this](){ SET(6,L) };
    cb_opcode_lookup[0xf6] = [this](){};
    cb_opcode_lookup[0xf7] = [this](){ SET(6,A) };
    cb_opcode_lookup[0xf8] = [this](){ SET(7,B) };
    cb_opcode_lookup[0xf9] = [this](){ SET(7,C) };
    cb_opcode_lookup[0xfa] = [this](){ SET(7,D) };
    cb_opcode_lookup[0xfb] = [this](){ SET(7,E) };
    cb_opcode_lookup[0xfc] = [this](){ SET(7,H) };
    cb_opcode_lookup[0xfd] = [this](){ SET(7,L) };
    cb_opcode_lookup[0xfe] = [this](){};
    cb_opcode_lookup[0xff] = [this](){ SET(7,A) };

}
