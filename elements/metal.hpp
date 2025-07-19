#ifndef ELEMENT_METAL_HPP
#define ELEMENT_METAL_HPP

#include <cmath>

// higher values, slower
#define ELEMENT_METAL_TEMP_ENV_LOSE_RATE 20
#define ELEMENT_METAL_TEMP_ENV_GAIN_RATE 15

namespace elements {

    void element_metal_update(int x, int y);

    void element_metal_spawn(int x, int y);

};

#include "../physics/common.hpp"
#include "elements.hpp"

#endif