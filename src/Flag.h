#pragma once
#include "utils.h"

class Flag{

public:

    bool Z, N, H, C;
    Flag();
    void reset();
    Register get_register() const;
    void set_register( Byte value );

};