#include "water.hpp"

namespace elements {

    void element_water_update(int x, int y) {
        struct element_water_properties *pr = (struct element_water_properties *) game::pixels[COORD(x,y)].custom_properties;

        physics::direction which_one;
        element_type which_element;
        physics::heat_update(x,y,ELEMENT_WATER_TEMP_ENV_LOSE_RATE,ELEMENT_WATER_TEMP_ENV_GAIN_RATE,ELEMENT_WATER_ENV_TEMP);
        //if(game::pixels[COORD(x,y)].physics_properties.temperature>ELEMENT_WATER_BOIL_TEMP) {
        if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>=ELEMENT_WATER_BOIL_TEMP) {
            // we turn into steam and destroy fire particle
            physics::clear_pixel(x,y);
        
            if(which_element==ELEMENT_FIRE) {
                switch(which_one) {
                    case physics::DIR_RIGHT:
                        physics::clear_pixel(x+1,y);
                        if(pr->salty>0&&game::rng_bool()) element_salt_spawn(x+1,y);
                        break;
                    case physics::DIR_LEFT:
                        physics::clear_pixel(x-1,y);
                        if(pr->salty>0&&game::rng_bool()) element_salt_spawn(x-1,y);
                        break;
                    case physics::DIR_DOWN:
                        physics::clear_pixel(x,y+1);
                        if(pr->salty>0&&game::rng_bool()) element_salt_spawn(x,y+1);
                        break;
                    case physics::DIR_UP:
                        physics::clear_pixel(x,y-1);
                        if(pr->salty>0&&game::rng_bool()) element_salt_spawn(x,y-1);
                        break;
                    case physics::DIR_NONE: // we are the hotest pixel!, there is no space to spawn salt
                        break;

                }
            }

            
            element_steam_spawn(x,y);
            return;
        }
        // if not salty check is there salt near, if yes: erase it and become salty
        if(pr->salty==0 && game::get_element_type(x-1,y) == ELEMENT_SALT) {
            physics::clear_pixel(x-1,y);
            element_water_become_salty(x,y,pr);
        }
        if(pr->salty==0 && game::get_element_type(x+1,y) == ELEMENT_SALT) {
            physics::clear_pixel(x+1,y);
            element_water_become_salty(x,y,pr);
        }
        if(pr->salty==0 && game::get_element_type(x,y-1) == ELEMENT_SALT) {
            physics::clear_pixel(x,y-1);
            element_water_become_salty(x,y,pr);
        }
        if(pr->salty==0 && game::get_element_type(x,y+1) == ELEMENT_SALT) {
            physics::clear_pixel(x,y+1);
            element_water_become_salty(x,y,pr);
        }

        physics::liquid_update(x, y);
    }

    void element_water_become_salty(int x, int y, struct element_water_properties *pr) {
        if(pr->salty>0) return;
        pr->salty = 1;
        game::pixels[COORD(x,y)].color.r = 48;
        game::pixels[COORD(x,y)].color.g = 96;
        game::pixels[COORD(x,y)].physics_properties.weight--;
    }

    void element_water_spawn(int x,int y) {
        // if there is solid/powder under, dont spawn
        if(physics::get_element_weight(x,y)>=100) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_WATER;
        new_pixel.color.r = 0;//255;
        if(game::rng_bool()) {
            new_pixel.color.g = 80;
        } else {
            new_pixel.color.g = 0;
        }
        new_pixel.color.b = 255;

        new_pixel.physics_properties.type = PHYSICS_LIQUID;
        new_pixel.physics_properties.weight = 100;
        new_pixel.physics_properties.temperature = ELEMENT_WATER_ENV_TEMP;
        new_pixel.physics_properties.destructable = 1; 
        new_pixel.physics_properties.flamable = ELEMENT_WATER_BOIL_TEMP; 

        struct element_water_properties *pr = (struct element_water_properties *) new_pixel.custom_properties;
        pr->salty = 0;

        game::pixels[COORD(x, y)] = new_pixel;
    }

    // steam
    
    void element_steam_update(int x, int y) {
        if(game::get_element_type(x,y-1) != ELEMENT_NONE && physics::get_element_weight(x,y-1)>50 && game::rng_bool()) {
            physics::clear_pixel(x,y);
            element_water_spawn(x,y);
            return;
        }
        physics::gas_update(x, y);
    }

    void element_steam_spawn(int x,int y) {
        // if there is solid/powder under, dont spawn
        if(physics::get_element_weight(x,y)>=100) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_STEAM;
        new_pixel.color.r = 150;
        new_pixel.color.g = 200;
        new_pixel.color.b = 230;

        new_pixel.physics_properties.type = PHYSICS_GAS;
        new_pixel.physics_properties.weight = 50;
        new_pixel.physics_properties.temperature = 40;
        new_pixel.physics_properties.destructable = 1;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
    
};