#include "CPU.h"
#include "InstructionSet.h"

Flag::Flag()
{
    reset();
}

void Flag::reset()
{
    Z = 0;
    N = 0;
    H = 0;
    C = 0;
}

Register Flag::get_register() const 
{
   Register temp = 0;
   temp = (Z << 7) + (N << 6) + (H << 5) + (C<<4);
   return temp;  
}

void Flag::set_register(Byte value) 
{
    Z = (bool)( value & (1<<7) );
    N = (bool)( value & (1<<6) );
    H = (bool)( value & (1<<5) );
    C = (bool)( value & (1<<4) ); 
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
void CPU::STOP()
{

}

void CPU::HALT()
{

}

void CPU::DI()
{
    p_mmu->write(0xffff, 0);
}

void CPU::EI()
{
    p_mmu->write(0xffff, 1);
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

void CPU::LD16_R(Register& hi, Register& lo, Pointer value )
{
    lo = (Byte)(0x00ff & value);
    hi = (Byte)(value >> 8);
}

void CPU::LD16_R(Pointer& r, Pointer value)
{
    r = value;
}

void CPU::LD16_P(Pointer address, Pointer value)
{
    Byte low = static_cast<Byte>(0xff & value);
    Byte high = static_cast<Byte>(value >> 8);
    p_mmu->write(address, low);
    p_mmu->write(address + 1, high);
}


void CPU::ADD( Byte value )
{
    Byte nibble_result = low_nibble(A) + low_nibble(value);
    if( high_nibble(nibble_result) > 0 ) F.H = true;

    Pointer result = A + value;
    if( high_byte(result) > 0 ) F.C = true;

    if( result == 0 ) F.Z = true;
    F.N = false;

    A = (Byte)result;
}

void CPU::SUB( Byte value )
{
    if( low_nibble(A) < low_nibble(value) ) F.H = true;
    if( A < value ) F.C = true;

    Pointer result = A - value;
    if( result == 0 ) F.Z = true;
    F.N = true;

    A = (Byte)result;
}

void CPU::AND( Byte value )
{
    A = A & value;
    if(A == 0) F.Z = true;
    F.N = false;
    F.H = true;
    F.C = false;
}

void CPU::OR ( Byte value )
{
    A = A | value;
    if(A == 0) F.Z = true;
    F.N = false;
    F.H = false;
    F.C = false;
}

void CPU::ADC( Byte value ){
    ADD( value + (Byte)(F.C) );
}

void CPU::SBC( Byte value ){
    SUB( value + (Byte)(F.C) );
}

void CPU::XOR( Byte value ){
    A = A ^ value;
    if( A == 0 ) F.Z = true;
    F.N = false;
    F.H = false;
    F.C = false;
}

void CPU::CP ( Byte value ){
    if( low_nibble(A) < low_nibble(value) ) F.H = true;
    if( A < value ) F.C = true;

    Pointer result = A - value;
    if( result == 0 ) F.Z = true;
    F.N = true;
}

void CPU::INC_R(Register& r)
{
    Byte nibble_result = low_nibble(r) + 1;
    if( high_nibble(nibble_result) > 0 ) F.H = true;

    r = r + 1;
    if( r == 0 ) F.Z = true;
    F.N = false;
}

void CPU::DEC_R(Register& r)
{
    if( low_nibble(r) == 0 ) F.H = true;

    r = r - 1;
    if( r == 0 ) F.Z = true;
    F.N = true;
}

void CPU::INC_P(Pointer p)
{
    Byte r = p_mmu->read(p);
    Byte nibble_result = low_nibble(r) + 1;
    if( high_nibble(nibble_result) > 0 ) F.H = true;

    r = r + 1;
    if( r == 0 ) F.Z = true;
    F.N = false;
    p_mmu->write(p, r);
}

void CPU::DEC_P(Pointer p)
{
    Byte r = p_mmu->read(p);
    if( low_nibble(r) == 0 ) F.H = true;

    r = r - 1;
    if( r == 0 ) F.Z = true;
    F.N = true;
    p_mmu->write(p, r);
}

void CPU::INC16(Register& hi, Register& lo)
{
    Pointer result = combine(hi, lo) + 1;
    LD16_R(hi, lo, result);
}

void CPU::INC16(Pointer& r)
{
    r++;
}

void CPU::DEC16(Register& hi, Register& lo)
{
    Pointer result = combine(hi, lo) - 1;
    LD16_R(hi, lo, result);
}

void CPU::DEC16(Pointer& r)
{
    r--;
}

void CPU::POP (Register& hi, Register& lo)
{
    Byte low = p_mmu->read(SP);
    Byte high = p_mmu->read(SP+1);
    SP += 2;
    LD16_R(hi, lo, combine(high,low) );
}

void CPU::PUSH( Pointer value )
{
    SP -= 2;
    LD16_P(SP, value);
}

// temporary fix
void CPU::POP_AF(){
    Byte low = p_mmu->read(SP);
    Byte high = p_mmu->read(SP+1);
    SP += 2;
    F.set_register(low);
    LD_R(A, high);
}

void CPU::JR( Condition c, Byte offset )
{
    JP( c, PC + offset );
}

void CPU::JP( Condition c, Pointer address )
{
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

void CPU::RET( Condition c )
{
    Byte hi, lo;
    POP(hi, lo);
    Pointer address = combine(hi, lo);
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
        break;
        default:
        break;
    }
}

void CPU::RST( Pointer offset ){
    PUSH( PC );
    JP( Condition::NONE, offset );
}