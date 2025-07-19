#include "petroleum.hpp"

namespace elements {

    void element_petroleum_update(int x, int y) {
        physics::direction which_one;
        element_type which_element;
        struct element_petroleum_properties *pr = (struct element_petroleum_properties *) game::pixels[COORD(x,y)].custom_properties;

        if(pr->burning) {
            pr->burn_time--;
            if(pr->burn_time<=1) {
                physics::clear_pixel(x,y);
                return;
            }
            if(game::rng_int(1,4)==1 && (game::get_element_type(x,y-1) == ELEMENT_NONE || game::get_element_type(x,y-1) == ELEMENT_FIRE)) {
                elements::element_spawn(x, y-1, ELEMENT_FIRE);
            }
        }
        
        if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>ELEMENT_PETROLEUM_IGNITE_TEMP) {
            // can only ignite if there is air around, or we are above petroleum which has air above
            if(!pr->burning && game::rng_bool() && (physics::can_ignite(x,y) || (game::get_element_type(x,y-1)==ELEMENT_PETROLEUM && game::get_element_type(x,y-2)==ELEMENT_NONE))) {
                game::pixels[COORD(x,y)].color.r = 240;
                game::pixels[COORD(x,y)].color.g = 205;
                game::pixels[COORD(x,y)].color.b = 82;
                game::pixels[COORD(x,y)].physics_properties.weight--;
                game::pixels[COORD(x,y)].physics_properties.temperature=ELEMENT_PETROLEUM_BURNING_TEMP;
                pr->burn_time = game::rng_int(36,64);
                pr->burning = true;
            }
        }
        
        physics::liquid_update(x, y);
    }

    void element_petroleum_spawn(int x,int y) {
        if(physics::get_element_weight(x,y)>=40) return;
        
        game::pixel_array_entry new_pixel;
        struct element_petroleum_properties *pr = (struct element_petroleum_properties *) new_pixel.custom_properties;

        new_pixel.id = ELEMENT_PETROLEUM;

        new_pixel.color.r = 180;
        new_pixel.color.g = 145;
        new_pixel.color.b = 32;
    

        new_pixel.physics_properties.type = PHYSICS_LIQUID;
        new_pixel.physics_properties.weight = 40;
        new_pixel.physics_properties.temperature = 15;
        new_pixel.physics_properties.destructable = 1;
        new_pixel.physics_properties.flamable = ELEMENT_PETROLEUM_IGNITE_TEMP; 

        pr->burning = false;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    

};