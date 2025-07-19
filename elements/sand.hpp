#ifndef ELEMENT_SAND_HPP
#define ELEMENT_SAND_HPP

#include <cmath>


namespace elements {

    void element_sand_update(int x, int y);
    
    void element_sand_spawn(int x, int y);
    
};


#include "../physics/powder.hpp"
#include "elements.hpp"

#endif