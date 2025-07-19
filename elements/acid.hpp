#ifndef ELEMENT_ACID_HPP
#define ELEMENT_ACID_HPP

#include <cmath>

#define ELEMENT_ACID_BOIL_TEMP 45

// higher values, slower
#define ELEMENT_ACID_TEMP_ENV_LOSE_RATE 5
#define ELEMENT_ACID_TEMP_ENV_GAIN_RATE 10
#define ELEMENT_ACID_ENV_TEMP 15


namespace elements {

    void element_acid_update(int x, int y);
    
    void element_acid_spawn(int x, int y);
    
    void element_acid_steam_update(int x, int y);
    
    void element_acid_steam_spawn(int x, int y);
};


#include "../physics/liquid.hpp"
#include "../physics/gas.hpp"
#include "../physics/heat.hpp"
#include "elements.hpp"

#endif