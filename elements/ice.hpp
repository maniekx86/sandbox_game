#ifndef ELEMENT_ICE_HPP
#define ELEMENT_ICE_HPP

#include <cmath>

#define ELEMENT_ICE_TEMP_ENV_LOSE_RATE 50
#define ELEMENT_ICE_TEMP_ENV_GAIN_RATE 80

namespace elements {

    void element_ice_update(int x, int y);

    void element_ice_spawn(int x, int y);

};


#include "../physics/common.hpp"
#include "../physics/heat.hpp"
#include "elements.hpp"

#endif