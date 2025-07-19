#include "fire.hpp"

namespace elements {

    // fire for some reason is very laggy
    void element_fire_update(int x, int y) {
        struct element_fire_properties *myprop = (struct element_fire_properties *) game::pixels[COORD(x,y)].custom_properties;


        if(myprop->burn_frame<=1) {
            physics::clear_pixel(x,y);
            return;
        }

        myprop->burn_frame--;
        if(game::rng_bool() && myprop->burn_frame > 2) myprop->burn_frame--;

        if(myprop->burn_frame > 65) {
            game::pixels[COORD(x,y)].color.r = 255;
            game::pixels[COORD(x,y)].color.b = 0;

            int val=255 - (99 - myprop->burn_frame) * (255 / 31);  // 96-65 = 31 steps
            if(val<0) val = 0;

            game::pixels[COORD(x,y)].color.g = val;
        } else {
            game::pixels[COORD(x,y)].color.b = 0;
            game::pixels[COORD(x,y)].color.g = 0;
            
            int val = 255 - (65 - myprop->burn_frame) * (255 / 65);
            if(val<10) val = 10;

            game::pixels[COORD(x,y)].color.r = val;
        }
        
        game::pixels[COORD(x,y)].physics_properties.temperature = 20 + (myprop->burn_frame / 2);

        int b_rand = game::rng_int(0,2) - 1;
        // if there is above us fire particle (+- rand) with lower burn frame, swap burn frames
        element_type element_above = game::get_element_type(x+b_rand,y-1);
        if(element_above==ELEMENT_FIRE) {
            struct element_fire_properties *upprop = (struct element_fire_properties *) game::pixels[COORD(x+b_rand,y-1)].custom_properties;
            if(upprop->burn_frame<myprop->burn_frame) {
                int temp = upprop->burn_frame;
                upprop->burn_frame = myprop->burn_frame;
                myprop->burn_frame = temp;
            }
        }
        if(element_above!=ELEMENT_NONE) {
            // if something blocks us, decrease timer faster
            if(myprop->burn_frame>4) myprop->burn_frame -= 4;
        }
        

        physics::gas_update(x, y);
    }

    void element_fire_spawn(int x,int y) {
        // if there is fire under us then we can replace it just
        if(game::get_element_type(x,y)!=ELEMENT_FIRE) {
            // if something is flamable and not solid then set its temperature to 70 if its flame point is less than
            if(game::get_element_type(x,y)!=ELEMENT_NONE) {
                if(game::pixels[COORD(x,y)].physics_properties.weight<200 && game::pixels[COORD(x,y)].physics_properties.flamable>0 && game::pixels[COORD(x,y)].physics_properties.flamable<=70)  {
                    game::pixels[COORD(x,y)].physics_properties.temperature = 70;
                }
                return;
            }
        }
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_FIRE;
        new_pixel.color.r = 255;
        new_pixel.color.g = 255;//-game::rng_int(0,127);
        new_pixel.color.b = 255;

        new_pixel.physics_properties.type = PHYSICS_GAS;
        new_pixel.physics_properties.weight = 20;
        new_pixel.physics_properties.temperature = 70;
        new_pixel.physics_properties.destructable = 1; 

        struct element_fire_properties *myprop = (struct element_fire_properties *) new_pixel.custom_properties;
        myprop->burn_frame = 100;

        game::pixels[COORD(x, y)] = new_pixel;
    }
    
};
