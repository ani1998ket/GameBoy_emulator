#include "CPU.h"

#define BC combine(B, C);
#define DE combine(D, E);
#define HL combine(H, L);

CPU::CPU()
{

}

void CPU::connect(MMU* p_mmu)
{
    this->p_mmu = p_mmu;
}

void CPU::reset()
{
    A = 0;
    B = 0;
    C = 0; 
    D = 0; 
    E = 0;
    F.Z = F.N = F.H = F.C = false;
    SP = 0;
    PC = 0;
}

Pointer combine(Register high, Register low)
{
    Pointer result = (high<<8) + low;
}