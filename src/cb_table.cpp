#include "CPU.h"

#define BC combine(B, C)
#define DE combine(D, E)
#define HL combine(H, L)

void CPU::init_cb(){
    cb_opcode_lookup[0x00] = this(){};
    cb_opcode_lookup[0x01] = this(){};
    cb_opcode_lookup[0x02] = this(){};
    cb_opcode_lookup[0x03] = this(){};
    cb_opcode_lookup[0x04] = this(){};
    cb_opcode_lookup[0x05] = this(){};
    cb_opcode_lookup[0x06] = this(){};
    cb_opcode_lookup[0x07] = this(){};
    cb_opcode_lookup[0x08] = this(){};
    cb_opcode_lookup[0x09] = this(){};
    cb_opcode_lookup[0x0a] = this(){};
    cb_opcode_lookup[0x0b] = this(){};
    cb_opcode_lookup[0x0c] = this(){};
    cb_opcode_lookup[0x0d] = this(){};
    cb_opcode_lookup[0x0e] = this(){};
    cb_opcode_lookup[0x0f] = this(){};

    cb_opcode_lookup[0x10] = this(){};
    cb_opcode_lookup[0x11] = this(){};
    cb_opcode_lookup[0x12] = this(){};
    cb_opcode_lookup[0x13] = this(){};
    cb_opcode_lookup[0x14] = this(){};
    cb_opcode_lookup[0x15] = this(){};
    cb_opcode_lookup[0x16] = this(){};
    cb_opcode_lookup[0x17] = this(){};
    cb_opcode_lookup[0x18] = this(){};
    cb_opcode_lookup[0x19] = this(){};
    cb_opcode_lookup[0x1a] = this(){};
    cb_opcode_lookup[0x1b] = this(){};
    cb_opcode_lookup[0x1c] = this(){};
    cb_opcode_lookup[0x1d] = this(){};
    cb_opcode_lookup[0x1e] = this(){};
    cb_opcode_lookup[0x1f] = this(){};

    cb_opcode_lookup[0x20] = this(){};
    cb_opcode_lookup[0x21] = this(){};
    cb_opcode_lookup[0x22] = this(){};
    cb_opcode_lookup[0x23] = this(){};
    cb_opcode_lookup[0x24] = this(){};
    cb_opcode_lookup[0x25] = this(){};
    cb_opcode_lookup[0x26] = this(){};
    cb_opcode_lookup[0x27] = this(){};
    cb_opcode_lookup[0x28] = this(){};
    cb_opcode_lookup[0x29] = this(){};
    cb_opcode_lookup[0x2a] = this(){};
    cb_opcode_lookup[0x2b] = this(){};
    cb_opcode_lookup[0x2c] = this(){};
    cb_opcode_lookup[0x2d] = this(){};
    cb_opcode_lookup[0x2e] = this(){};
    cb_opcode_lookup[0x2f] = this(){};
}
