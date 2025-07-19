#include "common.hpp"

namespace physics {

    void move_pixel(int old_x, int old_y, int new_x, int new_y) { // swap pixels
        struct game::pixel_array_entry temp_pixel;
        
        temp_pixel = game::pixels[COORD(old_x,old_y)];
        
        if(new_x<0 || new_x>=game::w || new_y<0 || new_y>=game::h) { // if out of bonds, delete the pixel
            clear_pixel(old_x, old_y);
        } else {
            game::pixels[COORD(old_x,old_y)] = game::pixels[COORD(new_x,new_y)];        
            game::pixels[COORD(new_x,new_y)] = temp_pixel;
        }
        
    }

    void clear_pixel(int x, int y) {
        struct game::pixel_array_entry new_pixel;
        new_pixel.id = ELEMENT_NONE;
        game::pixels[COORD(x,y)] = new_pixel;
    }

    int get_element_weight(int x, int y) {
        if(x<0 || x>=game::w || y<0 || y>=game::h) {
            if(game::borders_enabled) {
                return 255; 
            } else {
                return 0; 
            }
        }

        if(game::pixels[COORD(x,y)].id == ELEMENT_NONE) return 0;
        return game::pixels[COORD(x,y)].physics_properties.weight;
    }

    int get_element_temperature(int x, int y) {
        if(x<0 || x>=game::w || y<0 || y>=game::h) return 20;

        if(game::pixels[COORD(x,y)].id == ELEMENT_NONE) return 20;
        return game::pixels[COORD(x,y)].physics_properties.temperature;
    }

    int get_hottest_temperature_around(int x,int y, enum direction *which_one, element_type *which_element, bool skip_myself) {
        int max=-127;
        if(!skip_myself && get_element_temperature(x,y)>max) {
            max = get_element_temperature(x,y);
            *which_one = DIR_NONE;
            *which_element = game::get_element_type(x,y);
        }
        if(get_element_temperature(x+1,y)>max) {
            max = get_element_temperature(x+1,y);
            *which_one = DIR_RIGHT;
            *which_element = game::get_element_type(x+1,y);
        }
        if(get_element_temperature(x-1,y)>max) {
            max = get_element_temperature(x-1,y);
            *which_one = DIR_LEFT;
            *which_element = game::get_element_type(x-1,y);
        }
        if(get_element_temperature(x,y+1)>max) {
            max = get_element_temperature(x,y+1);
            *which_element = game::get_element_type(x,y+1);
            *which_one = DIR_DOWN;
        }
        if(get_element_temperature(x,y-1)>max) {
            max = get_element_temperature(x,y-1);
            *which_element = game::get_element_type(x,y-1);
            *which_one = DIR_UP;
        }
        return max;
    }

    int get_lowest_temperature_around(int x,int y, enum direction *which_one, element_type *which_element, bool skip_myself) {
        int min=127;
        if(!skip_myself && get_element_temperature(x,y)<min) {
            min = get_element_temperature(x,y);
            *which_one = DIR_NONE;
            *which_element = game::get_element_type(x,y);
        }
        if(get_element_temperature(x+1,y)<min) {
            min = get_element_temperature(x+1,y);
            *which_one = DIR_RIGHT;
            *which_element = game::get_element_type(x+1,y);
        }
        if(get_element_temperature(x-1,y)<min) {
            min = get_element_temperature(x-1,y);
            *which_one = DIR_LEFT;
            *which_element = game::get_element_type(x-1,y);
        }
        if(get_element_temperature(x,y+1)<min) {
            min = get_element_temperature(x,y+1);
            *which_element = game::get_element_type(x,y+1);
            *which_one = DIR_DOWN;
        }
        if(get_element_temperature(x,y-1)<min) {
            min = get_element_temperature(x,y-1);
            *which_element = game::get_element_type(x,y-1);
            *which_one = DIR_UP;
        }
        return min;
    }

    // basically the same as above, but does less checks if we need min and max
    void get_lowest_and_hottest_temperature_around(int x, int y, int *lowest, int *highest, direction *which_one_low, element_type *which_element_low, direction *which_one_hot, element_type *which_element_hot, bool skip_myself) {
        int temp=20;
        int min=127;
        int max=-127;
        if(!skip_myself) {
            temp = get_element_temperature(x,y);
            if(temp<min) {
                *lowest = min = temp;
                *which_one_low = DIR_NONE;
                *which_element_low = game::get_element_type(x,y);
            }
            if(temp>max) {
                *highest = max = temp;
                *which_one_hot = DIR_NONE;
                *which_element_hot = game::get_element_type(x,y);
            }
        }
        temp = get_element_temperature(x+1,y);
        if(temp<min) {
            *lowest = min = temp;
            *which_one_low = DIR_RIGHT;
            *which_element_low = game::get_element_type(x+1,y);
        }
        if(temp>max) {
            *highest = max = temp;
            *which_one_hot = DIR_RIGHT;
            *which_element_hot = game::get_element_type(x+1,y);
        }
        temp = get_element_temperature(x-1,y);
        if(temp<min) {
            *lowest = min = temp;
            *which_one_low = DIR_LEFT;
            *which_element_low = game::get_element_type(x-1,y);
        }
        if(temp>max) {
            *highest = max = temp;
            *which_one_hot = DIR_LEFT;
            *which_element_hot = game::get_element_type(x-1,y);
        }
        temp = get_element_temperature(x,y+1);
        if(temp<min) {
            *lowest = min = temp;
            *which_one_low = DIR_DOWN;
            *which_element_low = game::get_element_type(x,y+1);
        }
        if(temp>max) {
            *highest = max = temp;
            *which_one_hot = DIR_DOWN;
            *which_element_hot = game::get_element_type(x,y+1);
        }
        temp = get_element_temperature(x,y-1);
        if(temp<min) {
            *lowest = min = temp;
            *which_one_low = DIR_UP;
            *which_element_low = game::get_element_type(x,y-1);
        }
        if(temp>max) {
            *highest = max = temp;
            *which_one_hot = DIR_UP;
            *which_element_hot = game::get_element_type(x,y-1);
        }

    }

    bool can_ignite(int x, int y) {
        element_type ee[4];
        ee[0] = game::get_element_type(x-1, y);
        ee[1] = game::get_element_type(x+1, y);
        ee[2] = game::get_element_type(x, y-1);
        ee[3] = game::get_element_type(x, y+1);
        if((
            ee[0]== ELEMENT_NONE || ee[1] == ELEMENT_NONE || ee[2] == ELEMENT_NONE || ee[3] == ELEMENT_NONE ||
            ee[0]== ELEMENT_FIRE || ee[1] == ELEMENT_FIRE || ee[2] == ELEMENT_FIRE || ee[3] == ELEMENT_FIRE 
        )) return true;
        return false;
    }

    bool is_in_bounds(int x,int y) {
        if(x<0 || x>=game::w || y<0 || y>=game::h) return false;
        return true;
    }

};