#ifndef ELEMENT_FIRE_HPP
#define ELEMENT_FIRE_HPP

#include <cmath>
#include <stdint.h>

namespace elements {

    void element_fire_update(int x, int y);
    
    void element_fire_spawn(int x, int y);

    struct element_fire_properties {
        int8_t burn_frame;
    };
    
};


#include "../physics/gas.hpp"
#include "elements.hpp"

#endif