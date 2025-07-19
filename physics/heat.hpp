#ifndef PHYSICS_HEAT_HPP
#define PHYSICS_HEAT_HPP

#include "common.hpp"

namespace physics {
    // heat loss: every what frame lose heat
    // heat rate: every what frame gain heat
    inline int heat_framecounter=0;
    inline int heat_run_framecounter=0;
    inline int heat_run2_framecounter=0;

    void heat_update(int x, int y, int env_heat_loss_rate, int env_heat_gain_rate, int default_temp); 
    
};

#endif