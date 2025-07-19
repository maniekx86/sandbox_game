#include "liquid.hpp"

namespace physics {

    void liquid_update(int x, int y) {
        int my_weight = get_element_weight(x,y);
        // if element under has less than us weight, we go under
        // situation:
        //
        //  *
        // 
        //
        if(get_element_weight(x,y+1)<my_weight) {
            move_pixel(x, y, x, y+1);
            return;
        }

        // situation:
        //
        //  *
        //  #
        // ###
        // powder like spread, down left/right 
        int cc=0; // reduce checks
        if(get_element_weight(x-1,y+1)<my_weight) cc=cc+1;
        if(get_element_weight(x+1,y+1)<my_weight) cc=cc+2;
        if(cc==1) {// move left down
            move_pixel(x,y,x-1,y+1);
            return;
        }
        if(cc==2) {// move right down
            move_pixel(x,y,x+1,y+1);
            return;
        }
        if(cc==3) {// move left or right down randomly
            if(game::rng_bool()) {
                move_pixel(x,y,x-1,y+1);
            } else {
                move_pixel(x,y,x+1,y+1);
            }
            return;
        }


        // check down left/down right but further than 1 pixel
        element_type me = game::get_element_type(x,y);
        int max_xl, max_xr;
        // left check
        for(max_xl=2;max_xl<16;max_xl++) {
            if(game::get_element_type(x-max_xl,y+1)!=me) break;
        }
        if(game::get_element_type(x-max_xl,y+1)!=ELEMENT_NONE) max_xl=-1;
        // right check
        for(max_xr=2;max_xr<16;max_xr++) {
            if(game::get_element_type(x+max_xr,y+1)!=me) break;
        }
        if(game::get_element_type(x+max_xr,y+1)!=ELEMENT_NONE) max_xr=-1;
        if(max_xl>0 && max_xr >0) {
            // random
            if(game::rng_bool()) {
                move_pixel(x,y,x+max_xr,y+1);
            } else {
                move_pixel(x,y,x-max_xl,y+1);
            }
            return;
        }
        if(max_xl>0) {
            move_pixel(x,y,x-max_xl,y+1);
            return;
        }
        if(max_xr>0) {
            move_pixel(x,y,x+max_xr,y+1);
            return;
        }

        
        // move randomly left right

        cc=0; // reduce checks
        if(get_element_weight(x-1,y)<my_weight) cc=cc+1;
        if(get_element_weight(x+1,y)<my_weight) cc=cc+2;
        if(cc==1 && game::rng_bool()) {
            move_pixel(x,y,x-1,y);
            return;
        }
        if(cc==2 && game::rng_bool()) {
            move_pixel(x,y,x+1,y);
            return;
        }
        if(cc==3 && game::rng_bool()) {
            if(game::rng_bool()) {
                move_pixel(x,y,x-1,y);
            } else {
                move_pixel(x,y,x+1,y);
            }
            return;
        }
        
    }
    
};