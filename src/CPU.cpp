#include "CPU.h"
#include "InstructionSet.h"


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

void Flag::set_register(Byte value) {
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

void CPU::LD16_P(Pointer address, Pointer value)
{
    Byte low = static_cast<Byte>(0xff & value);
    Byte high = static_cast<Byte>(value >> 8);
    p_mmu->write(address, low);
    p_mmu->write(address + 1, high);
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
