#include "metal.hpp"

namespace elements {

    void element_metal_update(int x, int y) {
        physics::heat_update(x,y, ELEMENT_METAL_TEMP_ENV_LOSE_RATE, ELEMENT_METAL_TEMP_ENV_GAIN_RATE, 20);
        return; 
    }
    

    void element_metal_spawn(int x,int y) {
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_METAL;

        if(x%9==0 || y%9==0) {
                new_pixel.color.r=140;
                new_pixel.color.g=140;
                new_pixel.color.b=140;
            } else {
                new_pixel.color.r=180;
                new_pixel.color.g=180;
                new_pixel.color.b=180;
            }

        new_pixel.physics_properties.type = PHYSICS_NONE;
        new_pixel.physics_properties.weight = 200;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 0; 


        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};