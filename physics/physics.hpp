#ifndef PHYSICS_HPP
#define PHYSICS_HPP

// include here physics types //
#include "common.hpp"
#include "powder.hpp"
#include "liquid.hpp"
#include "gas.hpp"
////////////////////////////////

#include "heat.hpp"

#include <stdint.h>

enum physics_type : uint16_t {
    PHYSICS_NONE,
    PHYSICS_POWDER,
    PHYSICS_LIQUID,
    PHYSICS_GAS
};

// make sure to `make clean` if this changed so header files update for all object files
namespace physics { 
    struct properties { // maybe in future move to bits
        physics_type type;
        uint8_t weight;
        int8_t temperature = 20;
        int8_t destructable = 1;
        int8_t flamable = 0;
        int8_t dissolvable = 1;
    };
};


#endif