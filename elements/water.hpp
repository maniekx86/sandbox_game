#ifndef ELEMENT_WATER_HPP
#define ELEMENT_WATER_HPP

#include <cmath>
#include <stdint.h>

#define ELEMENT_WATER_BOIL_TEMP 45

// higher values, slower
#define ELEMENT_WATER_TEMP_ENV_LOSE_RATE 5
#define ELEMENT_WATER_TEMP_ENV_GAIN_RATE 10
#define ELEMENT_WATER_ENV_TEMP 15


namespace elements {

    struct element_water_properties {
        uint8_t salty;
    };

    void element_water_become_salty(int x, int y, struct element_water_properties *pr);

    void element_water_update(int x, int y);
    
    void element_water_spawn(int x, int y);
    
    void element_steam_update(int x, int y);
    
    void element_steam_spawn(int x, int y);
};


#include "../physics/liquid.hpp"
#include "../physics/gas.hpp"
#include "../physics/heat.hpp"
#include "elements.hpp"

#endif