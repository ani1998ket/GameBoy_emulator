#include "CPU.h"
#include "InstructionSet.h"

#define AF combine(A, F.get_register())
#define BC combine(B, C)
#define DE combine(D, E)
#define HL combine(H, L)


Flag::Flag(){
    reset();
}

void Flag::reset(){
    Z = 0;
    N = 0;
    H = 0;
    C = 0;
}

Register Flag::get_register() const {
   Register temp = 0;
   temp = (Z << 7) + (N << 6) + (H << 5) + (C<<4);
   return temp;  
}

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
    F.reset();
    H = 0;
    L = 0;
    SP = 0;
    PC = 0;
}

void CPU::step(){
    fetch();
    decode();
    execute();
}

Pointer CPU::combine(Byte hi, Byte lo)
{
    Pointer result = (hi<<8) + lo;
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

void CPU::NOP(){}
void CPU::STOP(){}
void CPU::HALT(){}
void CPU::DI(){}
void CPU::EI(){}

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

void CPU::LD16_R(Register& hi, Register& lo, Pointer value )
{
    lo = (Byte)(0x00ff & value);
    hi = (Byte)(value >> 8);
}

void CPU::LD16_R(Pointer& r, Pointer value)
{
    r = value;
}

void CPU::ADD( Byte value ){}
void CPU::SUB( Byte value ){}
void CPU::AND( Byte value ){}
void CPU::OR ( Byte value ){}
void CPU::ADC( Byte value ){}
void CPU::SBC( Byte value ){}
void CPU::XOR( Byte value ){}
void CPU::CP ( Byte value ){}
void CPU::INC_R(Register& r){}
void CPU::DEC_R(Register& r){}
void CPU::INC_P(Pointer p){}
void CPU::DEC_P(Pointer p){}
void CPU::INC16(Register& hi, Register& lo){}
void CPU::INC16(Pointer& r){}
void CPU::DEC16(Register& hi, Register& lo){}
void CPU::DEC16(Pointer& r){}
void CPU::POP (Register& hi, Register& lo){}
void CPU::PUSH( Pointer value ){}
// temporary fix
void CPU::POP_AF(){}

void CPU::JR( Condition c, Byte offset ){
    JP( c, PC + offset );
}
void CPU::JP( Condition c, Pointer address ){
    switch(c){
        case Condition::NZ:
            if( F.Z == false ) PC = address;
        break;
        case Condition::Z:
            if( F.Z == true ) PC = address;
        break;
        case Condition::NC:
            if( F.C == false ) PC = address;
        break;
        case Condition::C:
            if( F.C == true ) PC = address;
        break;
        case Condition::NONE:
            PC = address;
        break;
        default:
        break;
    }
}


void CPU::init(){
    opcode_lookup[0x00] = [this](){ NOP(); };
    opcode_lookup[0x10] = [this](){ STOP(); };
    opcode_lookup[0x20] = [this](){ JR(Condition::NZ, arg1); };
    opcode_lookup[0x30] = [this](){ JR(Condition::NC, arg1);};
    opcode_lookup[0x40] = [this](){ LD_R(B, B); };
    opcode_lookup[0x50] = [this](){ LD_R(D, B); };
    opcode_lookup[0x60] = [this](){ LD_R(H, B); };
    opcode_lookup[0x70] = [this](){ LD_P(HL,B); };
    opcode_lookup[0x80] = [this](){ ADD(B); };
    opcode_lookup[0x90] = [this](){ SUB(B); };
    opcode_lookup[0xa0] = [this](){ AND(B); };
    opcode_lookup[0xb0] = [this](){ OR (B); };
    opcode_lookup[0xc0] = [this](){};
    opcode_lookup[0xd0] = [this](){};
    opcode_lookup[0xe0] = [this](){ LD_P(0xff00 + arg1, A); };
    opcode_lookup[0xf0] = [this](){ LD_R(A, 0xff00 + arg1); };

    opcode_lookup[0x01] = [this](){ LD_R(B, arg2); LD_R(C, arg1); };
    opcode_lookup[0x11] = [this](){ LD_R(D, arg2); LD_R(E, arg1); };
    opcode_lookup[0x21] = [this](){ LD_R(H, arg2); LD_R(L, arg1); };
    opcode_lookup[0x31] = [this](){ LD16_R(SP, combine(arg2,arg1)); };
    opcode_lookup[0x41] = [this](){ LD_R(B, C); };
    opcode_lookup[0x51] = [this](){ LD_R(D, C); };
    opcode_lookup[0x61] = [this](){ LD_R(H, C); };
    opcode_lookup[0x71] = [this](){ LD_P(HL,C); };
    opcode_lookup[0x81] = [this](){ ADD(C); };
    opcode_lookup[0x91] = [this](){ SUB(C); };
    opcode_lookup[0xa1] = [this](){ AND(C); };
    opcode_lookup[0xb1] = [this](){ OR (C); };
    opcode_lookup[0xc1] = [this](){ POP(B, C); };
    opcode_lookup[0xd1] = [this](){ POP(D, E); };
    opcode_lookup[0xe1] = [this](){ POP(H, L); };
    opcode_lookup[0xf1] = [this](){ POP_AF(); };

    opcode_lookup[0x02] = [this](){ LD_P(BC, A); };
    opcode_lookup[0x12] = [this](){ LD_P(DE, A); };
    opcode_lookup[0x22] = [this](){ LD_P(HL, A); LD16_R(H, L, HL + 1); };
    opcode_lookup[0x32] = [this](){ LD_P(HL, A); LD16_R(H, L, HL - 1); };
    opcode_lookup[0x42] = [this](){ LD_R(B, D); };
    opcode_lookup[0x52] = [this](){ LD_R(D, D); };
    opcode_lookup[0x62] = [this](){ LD_R(H, D); };
    opcode_lookup[0x72] = [this](){ LD_P(HL,D); };
    opcode_lookup[0x82] = [this](){ ADD(D); };
    opcode_lookup[0x92] = [this](){ SUB(D); };
    opcode_lookup[0xa2] = [this](){ AND(D); };
    opcode_lookup[0xb2] = [this](){ OR (D); };
    opcode_lookup[0xc2] = [this](){ JP(Condition::NZ, combine(arg2, arg1)); };
    opcode_lookup[0xd2] = [this](){ JP(Condition::NC, combine(arg2, arg1)); };
    opcode_lookup[0xe2] = [this](){ LD_P(0xff00 + C, A); };
    opcode_lookup[0xf2] = [this](){ LD_R(A, 0xff00 + C); };

    opcode_lookup[0x03] = [this](){ INC16(B,C); };
    opcode_lookup[0x13] = [this](){ INC16(D,E); };
    opcode_lookup[0x23] = [this](){ INC16(H,L); };
    opcode_lookup[0x33] = [this](){ INC16(SP);  };
    opcode_lookup[0x43] = [this](){ LD_R(B, E); };
    opcode_lookup[0x53] = [this](){ LD_R(D, E); };
    opcode_lookup[0x63] = [this](){ LD_R(H, E); };
    opcode_lookup[0x73] = [this](){ LD_P(HL,E); };
    opcode_lookup[0x83] = [this](){ ADD(E); };
    opcode_lookup[0x93] = [this](){ SUB(E); };
    opcode_lookup[0xa3] = [this](){ AND(E); };
    opcode_lookup[0xb3] = [this](){ OR (E); };
    opcode_lookup[0xc3] = [this](){ JP(Condition::NONE, combine(arg2, arg1)); };
    opcode_lookup[0xd3] = [this](){};
    opcode_lookup[0xe3] = [this](){};
    opcode_lookup[0xf3] = [this](){};

    opcode_lookup[0x04] = [this](){ INC_R(B); };
    opcode_lookup[0x14] = [this](){ INC_R(D); };
    opcode_lookup[0x24] = [this](){ INC_R(H); };
    opcode_lookup[0x34] = [this](){ INC_P(HL); };
    opcode_lookup[0x44] = [this](){ LD_R(B, H); };
    opcode_lookup[0x54] = [this](){ LD_R(D, H); };
    opcode_lookup[0x64] = [this](){ LD_R(H, H); };
    opcode_lookup[0x74] = [this](){ LD_P(HL,H); };
    opcode_lookup[0x84] = [this](){ ADD(H); };
    opcode_lookup[0x94] = [this](){ SUB(H); };
    opcode_lookup[0xa4] = [this](){ AND(H); };
    opcode_lookup[0xb4] = [this](){ OR (H); };
    opcode_lookup[0xc4] = [this](){};
    opcode_lookup[0xd4] = [this](){};
    opcode_lookup[0xe4] = [this](){};
    opcode_lookup[0xf4] = [this](){};

    opcode_lookup[0x05] = [this](){ DEC_R(B); };
    opcode_lookup[0x15] = [this](){ DEC_R(D); };
    opcode_lookup[0x25] = [this](){ DEC_R(H); };
    opcode_lookup[0x35] = [this](){ DEC_P(HL); };
    opcode_lookup[0x45] = [this](){ LD_R(B, L); };
    opcode_lookup[0x55] = [this](){ LD_R(D, L); };
    opcode_lookup[0x65] = [this](){ LD_R(H, L); };
    opcode_lookup[0x75] = [this](){ LD_P(HL,L); };
    opcode_lookup[0x85] = [this](){ ADD(L); };
    opcode_lookup[0x95] = [this](){ SUB(L); };
    opcode_lookup[0xa5] = [this](){ AND(L); };
    opcode_lookup[0xb5] = [this](){ OR (L); };
    opcode_lookup[0xc5] = [this](){ PUSH(BC); };
    opcode_lookup[0xd5] = [this](){ PUSH(DE); };
    opcode_lookup[0xe5] = [this](){ PUSH(HL); };
    opcode_lookup[0xf5] = [this](){ PUSH(AF); };

    opcode_lookup[0x06] = [this](){ LD_R(B, arg1); };
    opcode_lookup[0x16] = [this](){ LD_R(D, arg1); };
    opcode_lookup[0x26] = [this](){ LD_R(H, arg1); };
    opcode_lookup[0x36] = [this](){ LD_P(HL,arg1); };
    opcode_lookup[0x46] = [this](){ LD_R_P(B, HL); };
    opcode_lookup[0x56] = [this](){ LD_R_P(D, HL); };
    opcode_lookup[0x66] = [this](){ LD_R_P(H, HL); };
    opcode_lookup[0x76] = [this](){ HALT(); };
    opcode_lookup[0x86] = [this](){ ADD(p_mmu->read(HL)); };
    opcode_lookup[0x96] = [this](){ SUB(p_mmu->read(HL)); };
    opcode_lookup[0xa6] = [this](){ AND(p_mmu->read(HL)); };
    opcode_lookup[0xb6] = [this](){ OR (p_mmu->read(HL)); };
    opcode_lookup[0xc6] = [this](){ ADD( arg1 ); };
    opcode_lookup[0xd6] = [this](){ SUB( arg1 ); };
    opcode_lookup[0xe6] = [this](){ AND( arg1 ); };
    opcode_lookup[0xf6] = [this](){ OR ( arg1 ); };

    opcode_lookup[0x07] = [this](){ };
    opcode_lookup[0x17] = [this](){ };
    opcode_lookup[0x27] = [this](){ };
    opcode_lookup[0x37] = [this](){ };
    opcode_lookup[0x47] = [this](){ LD_R(B, A); };
    opcode_lookup[0x57] = [this](){ LD_R(D, A); };
    opcode_lookup[0x67] = [this](){ LD_R(H, A); };
    opcode_lookup[0x77] = [this](){ LD_P(HL,A); };
    opcode_lookup[0x87] = [this](){ ADD(A); };
    opcode_lookup[0x97] = [this](){ SUB(A); };
    opcode_lookup[0xa7] = [this](){ AND(A); };
    opcode_lookup[0xb7] = [this](){ OR (A); };
    opcode_lookup[0xc7] = [this](){};
    opcode_lookup[0xd7] = [this](){};
    opcode_lookup[0xe7] = [this](){};
    opcode_lookup[0xf7] = [this](){};

    opcode_lookup[0x08] = [this](){ /* changes */ }; 
    opcode_lookup[0x18] = [this](){ JR(Condition::NONE, arg1); };
    opcode_lookup[0x28] = [this](){ JR(Condition::Z, arg1); };
    opcode_lookup[0x38] = [this](){ JR(Condition::C, arg1); };
    opcode_lookup[0x48] = [this](){ LD_R(C, B); };
    opcode_lookup[0x58] = [this](){ LD_R(E, B); };
    opcode_lookup[0x68] = [this](){ LD_R(L, B); };
    opcode_lookup[0x78] = [this](){ LD_P(A, B); };
    opcode_lookup[0x88] = [this](){ ADC(B); };
    opcode_lookup[0x98] = [this](){ SBC(B); };
    opcode_lookup[0xa8] = [this](){ XOR(B); };
    opcode_lookup[0xb8] = [this](){ CP (B); };
    opcode_lookup[0xc8] = [this](){};
    opcode_lookup[0xd8] = [this](){};
    opcode_lookup[0xe8] = [this](){};
    opcode_lookup[0xf8] = [this](){};

    opcode_lookup[0x09] = [this](){ }; 
    opcode_lookup[0x19] = [this](){ };
    opcode_lookup[0x29] = [this](){ };
    opcode_lookup[0x39] = [this](){ };
    opcode_lookup[0x49] = [this](){ LD_R(C, C); };
    opcode_lookup[0x59] = [this](){ LD_R(E, C); };
    opcode_lookup[0x69] = [this](){ LD_R(L, C); };
    opcode_lookup[0x79] = [this](){ LD_P(A, C); };
    opcode_lookup[0x89] = [this](){ ADC(C); };
    opcode_lookup[0x99] = [this](){ SBC(C); };
    opcode_lookup[0xa9] = [this](){ XOR(C); };
    opcode_lookup[0xb9] = [this](){ CP (C); };
    opcode_lookup[0xc9] = [this](){};
    opcode_lookup[0xd9] = [this](){};
    opcode_lookup[0xe9] = [this](){ JP(Condition::NONE, HL); };
    opcode_lookup[0xf9] = [this](){};

    opcode_lookup[0x0a] = [this](){ LD_R_P(A, BC); }; 
    opcode_lookup[0x1a] = [this](){ LD_R_P(A, DE); };
    opcode_lookup[0x2a] = [this](){ LD_R_P(A, HL); LD16_R(H, L, HL + 1); };
    opcode_lookup[0x3a] = [this](){ LD_R_P(A, HL); LD16_R(H, L, HL - 1); };
    opcode_lookup[0x4a] = [this](){ LD_R(C, D); };
    opcode_lookup[0x5a] = [this](){ LD_R(E, D); };
    opcode_lookup[0x6a] = [this](){ LD_R(L, D); };
    opcode_lookup[0x7a] = [this](){ LD_P(A, D); };
    opcode_lookup[0x8a] = [this](){ ADC(D); };
    opcode_lookup[0x9a] = [this](){ SBC(D); };
    opcode_lookup[0xaa] = [this](){ XOR(D); };
    opcode_lookup[0xba] = [this](){ CP (D); };
    opcode_lookup[0xca] = [this](){ JP(Condition::Z, combine(arg2, arg1)); };
    opcode_lookup[0xda] = [this](){ JP(Condition::C, combine(arg2, arg1)); };
    opcode_lookup[0xea] = [this](){};
    opcode_lookup[0xfa] = [this](){};

    opcode_lookup[0x0b] = [this](){ DEC16(B,C); }; 
    opcode_lookup[0x1b] = [this](){ DEC16(D,E); };
    opcode_lookup[0x2b] = [this](){ DEC16(H,L); };
    opcode_lookup[0x3b] = [this](){ DEC16(SP);  };
    opcode_lookup[0x4b] = [this](){ LD_R(C, E); };
    opcode_lookup[0x5b] = [this](){ LD_R(E, E); };
    opcode_lookup[0x6b] = [this](){ LD_R(L, E); };
    opcode_lookup[0x7b] = [this](){ LD_P(A, E); };
    opcode_lookup[0x8b] = [this](){ ADC(E); };
    opcode_lookup[0x9b] = [this](){ SBC(E); };
    opcode_lookup[0xab] = [this](){ XOR(E); };
    opcode_lookup[0xbb] = [this](){ CP (E); };
    opcode_lookup[0xcb] = [this](){};
    opcode_lookup[0xdb] = [this](){};
    opcode_lookup[0xeb] = [this](){};
    opcode_lookup[0xfb] = [this](){};

    opcode_lookup[0x0c] = [this](){ INC_R(C);  }; 
    opcode_lookup[0x1c] = [this](){ INC_R(E);  };
    opcode_lookup[0x2c] = [this](){ INC_R(L);  };
    opcode_lookup[0x3c] = [this](){ INC_R(A);  };
    opcode_lookup[0x4c] = [this](){ LD_R(C, H); };
    opcode_lookup[0x5c] = [this](){ LD_R(E, H); };
    opcode_lookup[0x6c] = [this](){ LD_R(L, H); };
    opcode_lookup[0x7c] = [this](){ LD_P(A, H); };
    opcode_lookup[0x8c] = [this](){ ADC(H); };
    opcode_lookup[0x9c] = [this](){ SBC(H); };
    opcode_lookup[0xac] = [this](){ XOR(H); };
    opcode_lookup[0xbc] = [this](){ CP (H); };
    opcode_lookup[0xcc] = [this](){};
    opcode_lookup[0xdc] = [this](){};
    opcode_lookup[0xec] = [this](){};
    opcode_lookup[0xfc] = [this](){};

    opcode_lookup[0x0d] = [this](){ DEC_R(C); }; 
    opcode_lookup[0x1d] = [this](){ DEC_R(E); };
    opcode_lookup[0x2d] = [this](){ DEC_R(L); };
    opcode_lookup[0x3d] = [this](){ DEC_R(A); };
    opcode_lookup[0x4d] = [this](){ LD_R(C, L); };
    opcode_lookup[0x5d] = [this](){ LD_R(E, L); };
    opcode_lookup[0x6d] = [this](){ LD_R(L, L); };
    opcode_lookup[0x7d] = [this](){ LD_P(A, L); };
    opcode_lookup[0x8d] = [this](){ ADC(L); };
    opcode_lookup[0x9d] = [this](){ SBC(L); };
    opcode_lookup[0xad] = [this](){ XOR(L); };
    opcode_lookup[0xbd] = [this](){ CP (L); };
    opcode_lookup[0xcd] = [this](){};
    opcode_lookup[0xdd] = [this](){};
    opcode_lookup[0xed] = [this](){};
    opcode_lookup[0xfd] = [this](){};

    opcode_lookup[0x0e] = [this](){ LD_R(C, arg1); }; 
    opcode_lookup[0x1e] = [this](){ LD_R(E, arg1); };
    opcode_lookup[0x2e] = [this](){ LD_R(L, arg1); };
    opcode_lookup[0x3e] = [this](){ LD_R(A ,arg1); };
    opcode_lookup[0x4e] = [this](){ LD_R(C, p_mmu->read(HL)); };
    opcode_lookup[0x5e] = [this](){ LD_R(E, p_mmu->read(HL)); };
    opcode_lookup[0x6e] = [this](){ LD_R(L, p_mmu->read(HL)); };
    opcode_lookup[0x7e] = [this](){ LD_P(A, p_mmu->read(HL)); };
    opcode_lookup[0x8e] = [this](){ ADC(p_mmu->read(HL)); };
    opcode_lookup[0x9e] = [this](){ SBC(p_mmu->read(HL)); };
    opcode_lookup[0xae] = [this](){ XOR(p_mmu->read(HL)); };
    opcode_lookup[0xbe] = [this](){ CP (p_mmu->read(HL)); };
    opcode_lookup[0xce] = [this](){ ADC( arg1 ); };
    opcode_lookup[0xde] = [this](){ SBC( arg1 ); };
    opcode_lookup[0xee] = [this](){ XOR( arg1 ); };
    opcode_lookup[0xfe] = [this](){ CP ( arg1 ); };

    opcode_lookup[0x0f] = [this](){ }; 
    opcode_lookup[0x1f] = [this](){ };
    opcode_lookup[0x2f] = [this](){ };
    opcode_lookup[0x3f] = [this](){ };
    opcode_lookup[0x4f] = [this](){ LD_R(C, A); };
    opcode_lookup[0x5f] = [this](){ LD_R(E, A); };
    opcode_lookup[0x6f] = [this](){ LD_R(L, A); };
    opcode_lookup[0x7f] = [this](){ LD_P(A, A); };
    opcode_lookup[0x8f] = [this](){ ADC(A); };
    opcode_lookup[0x9f] = [this](){ SBC(A); };
    opcode_lookup[0xaf] = [this](){ XOR(A); };
    opcode_lookup[0xbf] = [this](){ CP (A); };
    opcode_lookup[0xcf] = [this](){};
    opcode_lookup[0xdf] = [this](){};
    opcode_lookup[0xef] = [this](){};
    opcode_lookup[0xff] = [this](){};
    
}