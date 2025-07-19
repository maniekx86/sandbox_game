#include "salt.hpp"

namespace elements {

    void element_salt_update(int x, int y) {
        physics::powder_update(x, y);
    }

    void element_salt_spawn(int x,int y) {
        // if there is solid or powder under, dont spawn
        if(physics::get_element_weight(x,y)>=150) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_SALT;
        new_pixel.color.r = 225;
        new_pixel.color.g = 225;
        new_pixel.color.b = 225;

        new_pixel.physics_properties.type = PHYSICS_POWDER;
        new_pixel.physics_properties.weight = 150;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 1; 

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};