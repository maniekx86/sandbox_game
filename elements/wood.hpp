#ifndef ELEMENT_WOOD_HPP
#define ELEMENT_WOOD_HPP

#include <cmath>
#include <stdint.h>

#define ELEMENT_WOOD_FLAME_TEMP 45
#define ELEMENT_WOOD_BURNING_TEMP 60

namespace elements {

    struct element_wood_properties {
        uint16_t burntime;
        bool ignited;
    };

    void element_wood_update(int x, int y);

    void element_wood_spawn(int x, int y);

};

#include "../physics/common.hpp"
#include "elements.hpp"

#endif