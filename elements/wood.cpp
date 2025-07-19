#include "wood.hpp"

namespace elements {

    // no physics but some behaviour
    
    void element_wood_update(int x, int y) {
        physics::direction which_one;
        element_type which_element;
        struct element_wood_properties *pr = (struct element_wood_properties *) game::pixels[COORD(x,y)].custom_properties;

        if(pr->ignited) {
            if(pr->burntime>0) {
                pr->burntime--;
                if(game::rng_int(1,4)==4) {
                    // check where we can spawn fire
                    element_type element = game::get_element_type(x,y-1);
                    if(element == ELEMENT_NONE || element == ELEMENT_FIRE) { // prefer top
                        element_spawn(x, y-1, ELEMENT_FIRE);
                        return;
                    }
                    int cc=0;
                    if(game::get_element_type(x-1,y) == ELEMENT_NONE) { 
                        cc=cc+1;
                    }
                    if(game::get_element_type(x+1,y) == ELEMENT_NONE) { 
                        cc=cc+2;
                    }
                    if(cc==0) { // left and right occupied, try bottom, if there is no place, then we stop burning
                        if(game::get_element_type(x,y+1) == ELEMENT_NONE) {
                            element_spawn(x, y+1, ELEMENT_FIRE);
                        } else {
                            pr->ignited = false;
                            game::pixels[COORD(x,y)].physics_properties.temperature = 20;
                        }
                        return;
                    }
                    if(cc==1) element_spawn(x-1, y, ELEMENT_FIRE); // left
                    if(cc==2) element_spawn(x+1, y, ELEMENT_FIRE); // right
                    if(cc==3) { // random
                        if(game::rng_bool()) {
                            element_spawn(x+1, y, ELEMENT_FIRE);
                        } else {
                            element_spawn(x-1, y, ELEMENT_FIRE);
                        }
                    }
                }
            } else {
                physics::clear_pixel(x, y); // burntime = 0
            }
        } else {
            
            if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>=ELEMENT_WOOD_FLAME_TEMP) {
                // ignite if there is air around us (or fire) 
            
                if(game::rng_bool() && physics::can_ignite(x,y)) {
                    game::pixels[COORD(x,y)].physics_properties.temperature = ELEMENT_WOOD_BURNING_TEMP;
                    pr->ignited = 1;
                    if(pr->burntime==0) pr->burntime = game::rng_int(160,270); // wood can be ignited again
                    game::pixels[COORD(x,y)].color.r = 32;
                    game::pixels[COORD(x,y)].color.g = 32;
                    game::pixels[COORD(x,y)].color.b = 32;
                }
            }
        }

        return; 
    }
    

    void element_wood_spawn(int x,int y) {
        game::pixel_array_entry new_pixel;
        struct element_wood_properties *pr = (struct element_wood_properties *) new_pixel.custom_properties;
        // if there is wood under dont spawn
        if(game::get_element_type(x,y)==ELEMENT_WOOD) return;

        new_pixel.id = ELEMENT_WOOD;

        if((game::rng_int(1,10)>4&&y%2)||game::rng_int(1,10)>7) {
                new_pixel.color.r=165;
                new_pixel.color.g=120;
                new_pixel.color.b=71;
            } else {
                new_pixel.color.r=122;
                new_pixel.color.g=83;
                new_pixel.color.b=48;
            }

        new_pixel.physics_properties.type = PHYSICS_NONE;
        new_pixel.physics_properties.weight = 200;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 1; 
        new_pixel.physics_properties.flamable = ELEMENT_WOOD_FLAME_TEMP;

        pr->ignited = false;
        pr->burntime = 0;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};