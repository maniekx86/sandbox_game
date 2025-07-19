#include "brick.hpp"

namespace elements {

    // no physics
    /*
    void element_brick_update(int x, int y) {
        return; 
    }
    */

    void element_brick_spawn(int x,int y) {
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_BRICK;
        // brick pattern generator
        if((y%6==0) || (x%9==0&&y%12>6) || (x%9==5&&y%12<6)) {
            new_pixel.color.r=196;
            new_pixel.color.g=196;
            new_pixel.color.b=196;
        } else {
            new_pixel.color.r=196;
            new_pixel.color.g=0;
            new_pixel.color.b=0;
        }

        new_pixel.physics_properties.type = PHYSICS_NONE;
        new_pixel.physics_properties.weight = 200;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 1; 

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};