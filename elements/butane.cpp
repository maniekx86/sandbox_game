#include "butane.hpp"
#include <iostream>

namespace elements {

    void element_butane_update(int x, int y) {
        if(game::framecounter%2==y%2) {
            physics::direction which_one;
            element_type which_element;
            if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>=ELEMENT_BUTANE_IGNITE_TEMP) {
                physics::clear_pixel(x,y);
                elements::element_spawn(x, y, ELEMENT_FIRE);
            }
        }

        physics::gas_update(x, y);
    }

    void element_butane_spawn(int x,int y) {
        if(physics::get_element_weight(x,y)>=30) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_BUTANE;
        new_pixel.color.r = 32;
        new_pixel.color.g = 32;
        new_pixel.color.b = 32;

        new_pixel.physics_properties.type = PHYSICS_GAS;
        new_pixel.physics_properties.weight = 30;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 1; 
        new_pixel.physics_properties.flamable = ELEMENT_BUTANE_IGNITE_TEMP;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};