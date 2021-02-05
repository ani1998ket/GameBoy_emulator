#include "CPU.h"
#include "ISA.h"

#define BC combine(B, C)
#define DE combine(D, E)
#define HL combine(H, L)

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

void CPU::LD_R(Register& r, Byte value)
{
    r = value;
}
void CPU::LD_R_P(Register& r, Pointer address)
{
    r = p_mmu->read( address );
}

void CPU::LD_P(Pointer address, Byte value)
{
    p_mmu->write(address, value);
}

void CPU::init(){
    opcode_lookup[0x00] = [this](){};
    opcode_lookup[0x10] = [this](){};
    opcode_lookup[0x20] = [this](){};
    opcode_lookup[0x30] = [this](){};
    opcode_lookup[0x40] = [this](){ LD_R(B, B); };
    opcode_lookup[0x50] = [this](){ LD_R(D, B); };
    opcode_lookup[0x60] = [this](){ LD_R(H, B); };
    opcode_lookup[0x70] = [this](){ LD_P(HL, B); };
    opcode_lookup[0x80] = [this](){};
    opcode_lookup[0x90] = [this](){};
    opcode_lookup[0xa0] = [this](){};
    opcode_lookup[0xb0] = [this](){};
    opcode_lookup[0xc0] = [this](){};
    opcode_lookup[0xd0] = [this](){};
    opcode_lookup[0xe0] = [this](){ LD_P(0xff00 + arg1, A); };
    opcode_lookup[0xf0] = [this](){ LD_R(A, 0xff00 + arg1); };
}