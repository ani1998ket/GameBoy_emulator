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
    bool cb=0;

    Byte opcode;
    Byte arg1, arg2;
    std::function< void() > opcode_lookup[256];
    std::function< void() > cb_opcode_lookup[256];

private:

    void fetch();
    void decode();
    void execute();

    Pointer combine( Register hi, Register lo);
    void init();
    void init_cb();

    // 8 bit load commands
    void LD_R   (Register& r, Byte value);
    void LD_R_P (Register& r, Pointer address);
    void LD_P   (Pointer address, Byte value);

    // 16 bit load commands
    void LD16_R(Register& hi, Register& lo, Pointer value );
    void LD16_R(Pointer r, Pointer value);

    // ALU
    void ADD( Byte value );
    void SUB( Byte value );
    void AND( Byte value );
    void OR ( Byte value );
    void ADC( Byte value );
    void SBC( Byte value );
    void XOR( Byte value );
    void CP ( Byte value );

    void POP(Register& hi, Register& lo);

    void INC_R(Register& r);
    void INC_P(Pointer p);
    void DEC_R(Register& r);
    void DEC_P(Pointer p);

    //cb
    void RLC(Register &r);
    void RLC_16(Pointer &r);
    void RRC(Register &r);
    void RRC_16(Pointer &r);
    void RL (Register &r);
    void RL_16(Pointer &r);
    void RR (Register &r);
    void RR_16(Pointer &r);
    void SLA(Register &r);
    void SLA_16(Pointer &r);
    void SRA(Register &r);
    void SRA_16(Pointer &r);
    void SRL(Register &r);
    void SRL_16(Pointer &r);
    void BIT(int bit_ind, Register &r);
    void BIT_16(int bit_ind, Pointer &r);
    void SET(int bit_ind, Register &r);
    void SET_16(int bit_ind, Pointer &r);
    void RES(int bit_ind, Register &r);
    void RES_16(int bit_ind, Pointer &r);
    void SWAP(Register &r);
    void SWAP_16(Pointter &r);
};
