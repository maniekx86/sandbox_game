#ifndef ELEMENT_PETROLEUM_HPP
#define ELEMENT_PETROLEUM_HPP


#include <cmath>
#include <stdint.h>

#define ELEMENT_PETROLEUM_IGNITE_TEMP 45
#define ELEMENT_PETROLEUM_BURNING_TEMP 60

namespace elements {

    struct element_petroleum_properties {
        uint16_t burn_time;
        bool burning;
    };

    void element_petroleum_update(int x, int y);
    
    void element_petroleum_spawn(int x, int y);

};


#include "../physics/liquid.hpp"
#include "../physics/common.hpp"
#include "elements.hpp"

#endif