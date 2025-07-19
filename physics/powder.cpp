#include "powder.hpp"

namespace physics {

    void powder_update(int x, int y) {

        // if element under has less than us weight, we go under
        // situation:
        //
        //  *
        // 
        //
        if(get_element_weight(x,y+1)<get_element_weight(x,y)) {

            // rng between 2 nums if min, move left, if max, move right (if nothing is here) (freefall)
            int rng = game::rng_int(1,8);
            int o_x=0;
            

            if(rng==1) {
                if(game::get_element_type(x-1,y+1) == ELEMENT_NONE) o_x--;
            }
            if(rng==8) {
                if(game::get_element_type(x+1,y+1) == ELEMENT_NONE) o_x++;
            }

            move_pixel(x, y, x+o_x, y+1);

            return;
        }

        // situation:
        //
        //  *
        //  #
        // ###
        // powder like spread, down left/right 
        int cc=0; // reduce checks
        if(get_element_weight(x-1,y+1)<get_element_weight(x,y)) cc=cc+1;
        if(get_element_weight(x+1,y+1)<get_element_weight(x,y)) cc=cc+2;
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
        
        
    }
    
};