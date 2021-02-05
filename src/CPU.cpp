#include "CPU.h"
#include "ISA.h"

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
    return result;
}

void CPU::fetch()
{
    opcode = p_mmu->read(PC++);
}

void CPU::decode()
{
    int args = instruction_data[opcode].length - 1;
    if( args > 0 )
        arg1 = p_mmu->read(PC++);
    if( args > 1 )
        arg2 = p_mmu->read(PC++);
}

void CPU::execute()
{
    opcode_lookup[opcode]();
}