#include "CPU.h"
#include "InstructionSet.h"

CPU::CPU()
{

}

void CPU::connect(MMU* p_mmu)
{
    this->p_mmu = p_mmu;
}

void CPU::print_state(){
    cout << std::hex <<  "A : " << (int)A << "\t F : " << (int)F.get_register() << "\t B : " << (int)B << "\t C : " << (int)C << std::endl;
    cout << std::hex <<  "D : " << (int)D << "\t E : " << (int)E  << "\t H : " << (int)H << "\t L : " << (int)L << std::endl;
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

void CPU::step()
{
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
    if (opcode == 0xcb)
    {
        opcode = p_mmu->read(PC++);
        prefix_cb_used = true;;
    }
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
    if (prefix_cb_used == true)
    {
        cb_opcode_lookup[opcode]();
    }
    else
    {
        opcode_lookup[opcode]();
    }
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

void CPU::LD_HL_SP_n()
{
    F.Z = F.N = false;

    int8_t v = (int8_t)arg1;

    Pointer half_result = low_byte(SP) + v;
    F.H = (high_byte( half_result ) > 0);

    uint32_t result = SP + v;
    F.C = ( high_pointer(result) > 0 );

    LD16_R(H, L, low_pointer(result) );
}

void CPU::ADD( Byte value )
{
    Byte nibble_result = low_nibble(A) + low_nibble(value);
    F.H = ( high_nibble(nibble_result) > 0 );

    Pointer result = A + value;
    F.C = ( high_byte(result) > 0 );

    F.Z = ( result == 0 );
    F.N = false;

    A = (Byte)result;
}

void CPU::SUB( Byte value )
{
    F.H = ( low_nibble(A) < low_nibble(value) );
    F.C = ( A < value );

    Pointer result = A - value;
    F.Z = ( result == 0 );
    F.N = true;

    A = (Byte)result;
}

void CPU::AND( Byte value )
{
    A = A & value;
    F.Z = (A == 0);
    F.N = false;
    F.H = true;
    F.C = false;
}

void CPU::OR ( Byte value )
{
    A = A | value;
    F.Z = (A == 0);
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
    F.Z = ( A == 0 );
    F.N = false;
    F.H = false;
    F.C = false;
}

void CPU::CP ( Byte value ){
    F.H = ( low_nibble(A) < low_nibble(value) );
    F.C = ( A < value );

    Pointer result = A - value;
    F.Z = ( result == 0 );
    F.N = true;
}

void CPU::INC_R(Register& r)
{
    Byte nibble_result = low_nibble(r) + 1;
    F.H = ( high_nibble(nibble_result) > 0 );

    r = r + 1;
    F.Z = ( r == 0 );
    F.N = false;
}

void CPU::DEC_R(Register& r)
{
    F.H = ( low_nibble(r) == 0 );

    r = r - 1;
    F.Z = ( r == 0 );
    F.N = true;
}

void CPU::INC_P(Pointer p)
{
    Byte r = p_mmu->read(p);
    Byte nibble_result = low_nibble(r) + 1;
    F.H = ( high_nibble(nibble_result) > 0 );

    r = r + 1;
    F.Z = ( r == 0 );
    F.N = false;
    p_mmu->write(p, r);
}

void CPU::DEC_P(Pointer p)
{
    Byte r = p_mmu->read(p);
    F.H = ( low_nibble(r) == 0 );

    r = r - 1;
    F.Z = ( r == 0 );
    F.N = true;
    p_mmu->write(p, r);
}

void CPU::ADD16(Byte& hi, Byte& lo, Pointer v){
    F.N = false;

    Pointer v1 = combine(hi, lo);
    Pointer half_result = low_byte(v1) + low_byte(v);
    F.H = ( high_byte(half_result) > 0 );

    uint32_t result = v1 + v;
    F.C = ( high_pointer(result) > 0 );

    LD16_R(hi, lo, low_pointer(result) );
}

void CPU::ADD16(Pointer& r, Byte value){
    F.Z = F.N = false;
    int8_t v = (int8_t)value;

    Pointer half_result = low_byte(r) + v;
    F.H = ( high_byte( half_result ) > 0 );

    uint32_t result = r + v;
    F.C = ( high_pointer(result) > 0 );

    LD16_R(r, low_pointer(result) );
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

void CPU::DAA()
{
    /* Find implementation */
}

void CPU::SCF()
{
    F.N = false;
    F.H = false;
    F.C = true;
}

void CPU::CPL()
{
    A = A ^ 0xff; // Flip bits
    F.N = true;
    F.H = true;
}

void CPU::CCF()
{
    F.N = false;
    F.H = false;
    F.C = !F.C;
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
    int8_t signed_offset = offset;
    JP( c, PC + signed_offset );
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

void CPU::RST( Pointer offset )
{
    PUSH( PC - 1 ); // Current address
    JP( Condition::NONE, offset );
}

void CPU::CALL( Condition c, Pointer address )
{
    PUSH( PC ); // Next address
    JP( c, address );
}

