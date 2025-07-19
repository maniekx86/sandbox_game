#include "ice.hpp"


namespace elements {

    void element_ice_update(int x, int y) {
        //physics::direction which_one;
        //element_type which_element;

        physics::heat_update(x, y, ELEMENT_ICE_TEMP_ENV_LOSE_RATE, ELEMENT_ICE_TEMP_ENV_GAIN_RATE, -40);

        //if(physics::get_lowest_temperature_around(x, y, &which_one, &which_element)>0) {
        if(game::pixels[COORD(x, y)].physics_properties.temperature>0) {
            // we melt
            physics::clear_pixel(x, y);
            elements::element_spawn(x, y, ELEMENT_WATER);
            game::pixels[COORD(x,y)].physics_properties.temperature = 0;
        }

        return; 
    }
    

    void element_ice_spawn(int x,int y) {
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_ICE;
     
        new_pixel.color.r=96;
        new_pixel.color.g=192;
        new_pixel.color.b=224;


        new_pixel.physics_properties.type = PHYSICS_NONE;
        new_pixel.physics_properties.weight = 200;
        new_pixel.physics_properties.temperature = -40;
        new_pixel.physics_properties.destructable = 1; 

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};