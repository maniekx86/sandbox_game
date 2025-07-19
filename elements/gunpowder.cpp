#include "gunpowder.hpp"

namespace elements {

    void element_gunpowder_update(int x, int y) {
        physics::direction which_one;
        element_type which_element;
        if(game::framecounter%2==y%2) {// process every other line every other frame
            if(physics::get_hottest_temperature_around(x, y, &which_one, &which_element)>=ELEMENT_GUNPOWDER_EXPLODE_TEMP) {
                physics::clear_pixel(x, y);
                // spawn explosive
                element_explosive_particle_spawn(x, y);
            }
        }
        physics::powder_update(x, y);
    }

    void element_gunpowder_spawn(int x,int y) {
        // if there is solid or powder under, dont spawn
        if(physics::get_element_weight(x,y)>=150) return;
        
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_GUNPOWDER;
        if(game::rng_bool()) {
            new_pixel.color.r = 16;
            new_pixel.color.g = 16;
            new_pixel.color.b = 16;
        } else {
            new_pixel.color.r = 48;
            new_pixel.color.g = 48;
            new_pixel.color.b = 48;
        }

        new_pixel.physics_properties.type = PHYSICS_POWDER;
        new_pixel.physics_properties.weight = 150;
        new_pixel.physics_properties.temperature = 20;
        new_pixel.physics_properties.destructable = 1;
        new_pixel.physics_properties.flamable = ELEMENT_GUNPOWDER_EXPLODE_TEMP; 

        game::pixels[COORD(x, y)] = new_pixel;
    }
    

    void element_explosive_particle_update(int x, int y) {
        // custom physics only used here
        // if its gonna be used somewhere else then i gonna move it to physics folder
        struct element_explosive_particle_properties *pr = (struct element_explosive_particle_properties *) game::pixels[COORD(x,y)].custom_properties;

        if(pr->last_update_frame == (game::framecounter & 0xFF)) {
            return; // we already updated this frame 
            // this fixes problem of canvas update from bottom to the up
        }
        pr->last_update_frame = game::framecounter & 0xFF;

        if(pr->live_time<=1) {
            physics::clear_pixel(x,y); // we die
            return;
        }
        pr->live_time--;
        if(pr->live_time>3 && game::rng_bool()) pr->live_time--;
        if(pr->live_time>2 && game::rng_bool()) pr->live_time--;

        game::pixels[COORD(x,y)].physics_properties.temperature = 50 + (pr->live_time/4);

        int rand_move=0;
        if(game::rng_int(1,8)==8) {
            if(game::rng_bool()) {
                rand_move = -1;
            } else {
                rand_move = 1;
            }
        }
        
        // in rare cases we can change direction
        if(game::rng_int(1,2)==1) {
            element_explosive_direction rand_dirs[3];
            
            switch(pr->origin_direction) {
            case EXD_DIR_LEFT:
                rand_dirs[0] = EXD_DIR_LEFT; // match first as original dir
                rand_dirs[1] = EXD_DIR_LEFT_DOWN;
                rand_dirs[2] = EXD_DIR_LEFT_UP;
                break;
            case EXD_DIR_RIGHT:
                rand_dirs[0] = EXD_DIR_RIGHT;
                rand_dirs[1] = EXD_DIR_RIGHT_DOWN;
                rand_dirs[2] = EXD_DIR_RIGHT_UP;
                break;
            case EXD_DIR_UP:
                rand_dirs[0] = EXD_DIR_UP;
                rand_dirs[1] = EXD_DIR_RIGHT_UP;
                rand_dirs[2] = EXD_DIR_LEFT_UP;
                break;
            case EXD_DIR_DOWN:
                rand_dirs[0] = EXD_DIR_DOWN;
                rand_dirs[1] = EXD_DIR_RIGHT_DOWN;
                rand_dirs[2] = EXD_DIR_LEFT_DOWN;
                break;
            case EXD_DIR_LEFT_DOWN:
                rand_dirs[0] = EXD_DIR_LEFT_DOWN;
                rand_dirs[1] = EXD_DIR_LEFT;
                rand_dirs[2] = EXD_DIR_DOWN;
                break;
            case EXD_DIR_LEFT_UP:
                rand_dirs[0] = EXD_DIR_LEFT_UP;
                rand_dirs[1] = EXD_DIR_LEFT;
                rand_dirs[2] = EXD_DIR_UP;
                break;
            case EXD_DIR_RIGHT_DOWN:
                rand_dirs[0] = EXD_DIR_RIGHT_DOWN;
                rand_dirs[1] = EXD_DIR_RIGHT;
                rand_dirs[2] = EXD_DIR_DOWN;
                break;
            case EXD_DIR_RIGHT_UP:
                rand_dirs[0] = EXD_DIR_RIGHT_UP;
                rand_dirs[1] = EXD_DIR_RIGHT;
                rand_dirs[2] = EXD_DIR_UP;
                break;
            }
            
            int random_val = game::rng_int(0,3);
            if(random_val>2) random_val = 0; // more chances for [0]

            pr->direction = rand_dirs[random_val];
            
        }

        int t_x = 0;
        int t_y = 0;
        //bool diag_rand=false;

        switch(pr->direction) {
            case EXD_DIR_LEFT:
                t_x = -1;
                t_y = rand_move;
                break;
            case EXD_DIR_RIGHT:
                t_x = 1;
                t_y = rand_move;
                break;
            case EXD_DIR_UP:
                t_x = rand_move;
                t_y = -1;
                break;
            case EXD_DIR_DOWN:
                t_x = rand_move;
                t_y = 1;
                break;
            case EXD_DIR_LEFT_DOWN:
                t_x = -1;
                t_y = 1;
                if(rand_move!=0) t_x = 0;
                //diag_rand = true;
                break;
            case EXD_DIR_LEFT_UP:
                t_x = -1;
                t_y = -1;
                if(rand_move!=0) t_x = 0;
                //diag_rand = true;
                break;
            case EXD_DIR_RIGHT_DOWN:
                t_x = 1;
                t_y = 1;
                if(rand_move!=0) t_x = 0;
                //diag_rand = true;
                break;
            case EXD_DIR_RIGHT_UP:
                t_x = 1;
                t_y = -1;
                if(rand_move!=0) t_x = 0;
                //diag_rand = true;
                break;
        }
        // rand move for diagonal move
        // note: setting only x to 0 rather than randomizing to set x or y makes explosion look better
        // if(diag_rand) {
        //     if(rand_move==-1) {
        //         t_x = 0;
        //     } else if(rand_move == 1) {
        //         t_y = 0;
        //     }
        // }

        if(!physics::is_in_bounds(x+t_x,y+t_y)) { // out of bounds
            physics::clear_pixel(x,y);
            return;    
        }
        if(game::get_element_type(x+t_x,y+t_y) == ELEMENT_NONE || game::pixels[COORD(x+t_x,y+t_y)].physics_properties.destructable) {
            physics::clear_pixel(x+t_x,y+t_y);
            physics::move_pixel(x,y,x+t_x,y+t_y);
            if(game::rng_bool()) elements::element_spawn(x, y, ELEMENT_FIRE);
        } else {
            physics::clear_pixel(x, y); // we die
        }
    

        
    }

    void element_explosive_particle_spawn(int x, int y) {
        game::pixel_array_entry new_pixel;

        new_pixel.id = ELEMENT_EXPLOSIVE_PARTCILE;
        new_pixel.color.r = 255;
        new_pixel.color.g = 255;
        new_pixel.color.b = 255;

        new_pixel.physics_properties.type = PHYSICS_NONE;
        new_pixel.physics_properties.weight = 200; // should not be pushed
        new_pixel.physics_properties.destructable = 1; 

        struct element_explosive_particle_properties *pr = (struct element_explosive_particle_properties *) new_pixel.custom_properties;

        switch(game::rng_int(0,7)) {
            case 0:
                pr->direction = EXD_DIR_LEFT;
                break;
            case 1:
                pr->direction = EXD_DIR_RIGHT;
                break;
            case 2:
                pr->direction = EXD_DIR_UP;
                break;
            case 3:
                pr->direction = EXD_DIR_DOWN;
                break;
            case 4:
                pr->direction = EXD_DIR_LEFT_DOWN;
                break;
            case 5:
                pr->direction = EXD_DIR_LEFT_UP;
                break;
            case 6:
                pr->direction = EXD_DIR_RIGHT_DOWN;
                break;
            case 7:
                pr->direction = EXD_DIR_RIGHT_UP;
                break;
        }
        pr->origin_direction = pr->direction;

        pr->last_update_frame = game::framecounter & 0xFF;
        pr->live_time = 127;

        new_pixel.physics_properties.temperature = 55 + (127/4); // peak temperature 86
        

        game::pixels[COORD(x, y)] = new_pixel;
    }
};
