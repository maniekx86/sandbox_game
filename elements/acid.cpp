#include "acid.hpp"

namespace elements {

    void element_acid_update(int x, int y) {
        physics::direction which_one;
        element_type which_element;
        physics::heat_update(x, y, ELEMENT_ACID_TEMP_ENV_LOSE_RATE, ELEMENT_ACID_TEMP_ENV_GAIN_RATE, ELEMENT_ACID_ENV_TEMP);
        
        if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>=ELEMENT_ACID_BOIL_TEMP) {
            physics::clear_pixel(x,y);
            element_acid_steam_spawn(x,y);
            return;
        }

        // randomly decide which pixel to check and destroy it if its not acid or water
        // also if its water, randomly decide to move to it
        int t_x, t_y;
        switch(game::rng_int(0,3)) {
            case 0: // right
                t_x = 1;
                t_y = 0;
                break;
            case 1: // left
                t_x = -1;
                t_y = 0;
                break;
            case 2: // bottom
                t_x = 0;
                t_y = 1;
                break;
            case 3: // top
                t_x = 0;
                t_y = -1;
                break;
            default:
                t_x = 0;
                t_y = 0;
                break;
        }
        if(game::get_element_type(x+t_x, y+t_y)==ELEMENT_WATER) {
            if(game::rng_int(1,4)==1) {
                physics::move_pixel(x, y, x+t_x, y+t_y);
            } else {
                physics::liquid_update(x, y);
            }
            return;
        }
        // we can dissolve if: in bounds and not gas
        if(game::get_element_type(x+t_x, y+t_y)!=ELEMENT_ACID && game::get_element_type(x+t_x, y+t_y)!=ELEMENT_NONE) {
            if(
                game::rng_bool() && physics::is_in_bounds(x+t_x, y+t_y) && 
                game::pixels[COORD(x+t_x,y+t_y)].physics_properties.type != PHYSICS_GAS && 
                game::pixels[COORD(x+t_x,y+t_y)].physics_properties.dissolvable>0) 
            {
                physics::clear_pixel(x,y);
                physics::clear_pixel(x+t_x, y+t_y);
                return;
            }
        }

        physics::liquid_update(x, y);
    }

    void element_acid_spawn(int x,int y) {
        // if there is solid/powder under, dont spawn
        if(physics::get_element_weight(x,y)>=100) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_ACID;
        new_pixel.color.r = 96;
        new_pixel.color.g = 255;
        new_pixel.color.b = 96;

        new_pixel.physics_properties.type = PHYSICS_LIQUID;
        new_pixel.physics_properties.weight = 100;
        new_pixel.physics_properties.temperature = ELEMENT_ACID_ENV_TEMP;
        new_pixel.physics_properties.destructable = 1; 
        new_pixel.physics_properties.flamable = ELEMENT_ACID_BOIL_TEMP; 
        new_pixel.physics_properties.dissolvable = 0;

        game::pixels[COORD(x, y)] = new_pixel;
    }

    // acid steam
    
    void element_acid_steam_update(int x, int y) {
        if(game::get_element_type(x,y-1) != ELEMENT_NONE && physics::get_element_weight(x,y-1)>50 && game::rng_bool()) {
            physics::clear_pixel(x,y);
            element_acid_spawn(x,y);
            return;
        }
        physics::gas_update(x, y);
    }

    void element_acid_steam_spawn(int x,int y) {
        // if there is solid/powder under, dont spawn
        if(physics::get_element_weight(x,y)>=100) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_ACID_STEAM;
        new_pixel.color.r = 140;
        new_pixel.color.g = 255;
        new_pixel.color.b = 140;

        new_pixel.physics_properties.type = PHYSICS_GAS;
        new_pixel.physics_properties.weight = 50;
        new_pixel.physics_properties.temperature = 40;
        new_pixel.physics_properties.destructable = 1;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
    
};