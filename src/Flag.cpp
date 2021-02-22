#include "Flag.h"

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
