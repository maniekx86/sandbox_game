#ifndef ELEMENT_GUNPOWDER_HPP
#define ELEMENT_GUNPOWDER_HPP


#include <cmath>
#include <stdint.h>

#define ELEMENT_GUNPOWDER_EXPLODE_TEMP 45

namespace elements {

    void element_gunpowder_update(int x, int y);
    
    void element_gunpowder_spawn(int x, int y);

    enum element_explosive_direction : uint8_t {
        EXD_DIR_LEFT,
        EXD_DIR_RIGHT,
        EXD_DIR_DOWN,
        EXD_DIR_UP,

        EXD_DIR_LEFT_UP,
        EXD_DIR_LEFT_DOWN,
        EXD_DIR_RIGHT_UP,
        EXD_DIR_RIGHT_DOWN
    };

    struct element_explosive_particle_properties {
        uint8_t last_update_frame;
        uint8_t live_time;
        element_explosive_direction direction;
        element_explosive_direction origin_direction;
    };

    
    void element_explosive_particle_update(int x, int y);

    void element_explosive_particle_spawn(int x, int y);
};


#include "../physics/powder.hpp"
#include "../physics/common.hpp"
#include "elements.hpp"

#endif
