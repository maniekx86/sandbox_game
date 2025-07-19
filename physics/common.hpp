#ifndef PHYSICS_COMMON_HPP
#define PHYSICS_COMMON_HPP

#include <SFML/Graphics.hpp>
#include <stdint.h>
#include "physics.hpp"
#include "../game.hpp"
#include "../elements/elements.hpp"

namespace physics {

    enum direction {
        DIR_LEFT,
        DIR_RIGHT,
        DIR_DOWN,
        DIR_UP,
        DIR_NONE
    };

    void move_pixel(int old_x, int old_y, int new_x, int new_y);
    void clear_pixel(int x, int y);
    int get_element_weight(int x, int y);
    int get_element_temperature(int x, int y);
    int get_hottest_temperature_around(int x, int y, direction *which_one, element_type *which_element, bool skip_myself=false);
    int get_lowest_temperature_around(int x, int y, direction *which_one, element_type *which_element, bool skip_myself=false);
    void get_lowest_and_hottest_temperature_around(int x, int y, int *lowest, int *highest, direction *which_one_low, element_type *which_element_low, direction *which_one_hot, element_type *which_element_hot, bool skip_myself=false);
    bool is_in_bounds(int x,int y);
    bool can_ignite(int x, int y); // checks if pixel is not surrounded

};


#endif