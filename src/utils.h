#pragma once

#include <cstdint>

using Byte = uint8_t;
using Pointer = uint16_t;
using Register = uint8_t; 

inline Byte low_nibble ( Byte value ){
    return (Byte)( 0x0f & value ); 
}

inline Byte high_nibble(Byte value ){
    return (Byte)( value >> 4 );
}

inline Byte low_byte( Pointer value ){
    return (Byte)( 0xff & value ); 
}

inline Byte high_byte( Pointer value ){
    return (Byte)( value >> 8 );
}

inline Pointer low_pointer( uint32_t value ){
    return (Pointer)( 0xffff & value ); 
}

inline Pointer high_pointer( uint32_t value ){
    return (Pointer)( value >> 16 );
}