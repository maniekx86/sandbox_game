#include "heat.hpp"
#include <iostream>

namespace physics {

    /*
    BUG:
    for some reason if env_heat_xxxx_rate is set to even number temperature doesnt drop
    */

    void heat_update(int x, int y, int env_heat_loss_rate, int env_heat_gain_rate, int default_temp) {
        int our_temp = game::pixels[COORD(x,y)].physics_properties.temperature;
        int hottest_temp;
        int lowest_temp;
        // update every other line (swap on frames) to prevent instant heating from bottom to top
        if(heat_run_framecounter==2) {
            heat_run2_framecounter++;
            if(heat_run2_framecounter>5) {
                heat_run2_framecounter = 0;
                heat_run_framecounter = 0;
            }
            if(heat_run2_framecounter>1) return;
        }
        if(y%2==game::framecounter%2) {
            heat_run_framecounter++;
            heat_framecounter++;
            

            if(our_temp>default_temp) {
                if(heat_framecounter % env_heat_loss_rate == 0) our_temp=our_temp-1;
            } 
            if(our_temp<default_temp) {
                if(heat_framecounter % env_heat_gain_rate == 0) our_temp=our_temp+1;
            }
            physics::direction which_one;
            element_type which_element, which_element_min;
            //if(game::framecounter%2>0) goto heat_finish;

            //hottest_temp = physics::get_hottest_temperature_around(x, y, &which_one, &which_element, true);
            //lowest_temp  = physics::get_lowest_temperature_around(x, y, &which_one, &which_element_min, true);
            physics::get_lowest_and_hottest_temperature_around(x, y, &lowest_temp, &hottest_temp, &which_one, &which_element_min, &which_one, &which_element, true); 
            // heat gain
            
            bool allow_gain=true; // favour cold materials
            if(our_temp<0 && heat_framecounter%3!=0) allow_gain = false;
            if(allow_gain && (hottest_temp)>our_temp+1) {
                int gain_rate;
                if(our_temp<0) {
                    gain_rate = (hottest_temp - our_temp)/20;
                } else {
                    gain_rate = (hottest_temp - our_temp)/10;
                }
                if(gain_rate==0) gain_rate = 1;
                our_temp = our_temp + gain_rate;
                //our_temp++;
                //goto heat_finish;
            }
            // heat lose
            
            if(heat_framecounter%2==0) {
                if(lowest_temp<our_temp-1) {
                    // if air, then we lose temp slower
                    int gain_rate;
                    if(which_element_min==ELEMENT_NONE) {
                        gain_rate = (our_temp - hottest_temp)/20;
                    } else {
                        gain_rate = (our_temp - hottest_temp)/10;
                    }
                    
                    if(gain_rate==0) gain_rate = 1;
                    our_temp = our_temp - gain_rate;
                    //our_temp--;
                    //goto heat_finish;
                }
            }
        } 
        
        //heat_finish:
        game::pixels[COORD(x,y)].physics_properties.temperature = our_temp;
    }
    
};