#ifndef ELEMENT_BUTANE_HPP
#define ELEMENT_BUTANE_HPP

#include <cmath>

#define ELEMENT_BUTANE_IGNITE_TEMP 45

namespace elements {

    void element_butane_update(int x, int y);
    
    void element_butane_spawn(int x, int y);
    
};


#include "../physics/gas.hpp"
#include "elements.hpp"

#endif