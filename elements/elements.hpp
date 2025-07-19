// How to add element:
/*
    1. create cpp/hpp file for it and write code for it
    for template see sand or brick code
    2. include it in this file bellow
    3. register it in element_type enum
    4. register it in element_update() and element_spawn() (only element_spawn is necceseray, if your element is static you can skip it)
    5. register it at game.cpp in register_elements()
*/

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include "../game.hpp"

// elements include
#include "sand.hpp"
#include "brick.hpp"
#include "water.hpp" // water + steam
#include "fire.hpp"
#include "salt.hpp"
#include "gunpowder.hpp" // gunpowder + explosive particle
#include "indestructible_wall.hpp"
#include "wood.hpp"
#include "metal.hpp"
#include "petroleum.hpp"
#include "butane.hpp"
#include "acid.hpp" // acid + acid steam
#include "ice.hpp"
///


#include <stdint.h>


enum element_type : uint16_t {
    ELEMENT_NONE,
    ELEMENT_BORDER,
    ELEMENT_SAND,
    ELEMENT_BRICK,
    ELEMENT_WATER,
    ELEMENT_STEAM, // coded at water
    ELEMENT_FIRE,
    ELEMENT_SALT,
    ELEMENT_GUNPOWDER,
    ELEMENT_EXPLOSIVE_PARTCILE, // coded at gunpowder
    ELEMENT_INDESTRUCTIBLE_WALL,
    ELEMENT_WOOD,
    ELEMENT_ACID,
    ELEMENT_BUTANE,
    ELEMENT_METAL,
    ELEMENT_PETROLEUM,
    ELEMENT_ACID_STEAM,
    ELEMENT_ICE
};

namespace elements {

    // this function is ran when the pixel is updated
    inline void element_update(int x, int y, element_type element) { 

        switch(element) {
            case ELEMENT_NONE:
                return;
            case ELEMENT_BORDER:
                return;
            case ELEMENT_SAND: 
                elements::element_sand_update(x, y);
                break;
            case ELEMENT_BRICK: 
                //elements::element_brick_update(x, y);
                break;
            case ELEMENT_WATER:
                elements::element_water_update(x, y);
                break;
            case ELEMENT_STEAM:
                elements::element_steam_update(x, y);
                break;
            case ELEMENT_FIRE:
                elements::element_fire_update(x, y);
                break;
            case ELEMENT_SALT:
                elements::element_salt_update(x, y);
                break;
            case ELEMENT_GUNPOWDER:
                elements::element_gunpowder_update(x, y);
                break;
            case ELEMENT_EXPLOSIVE_PARTCILE:
                elements::element_explosive_particle_update(x, y);
                break;
            case ELEMENT_INDESTRUCTIBLE_WALL:
                break;
            case ELEMENT_WOOD:
                elements::element_wood_update(x, y);
                break;
            case ELEMENT_METAL:
                elements::element_metal_update(x, y);
                break;
            case ELEMENT_PETROLEUM:
                elements::element_petroleum_update(x, y);
                break;
            case ELEMENT_BUTANE:
                elements::element_butane_update(x, y);
                break;
            case ELEMENT_ACID:
                elements::element_acid_update(x, y);
                break;
            case ELEMENT_ACID_STEAM:
                elements::element_acid_steam_update(x, y);
                break;
            case ELEMENT_ICE:
                elements::element_ice_update(x, y);
                break;
        }
    }

    // this function is ran when the pixel is created
    inline void element_spawn(int x, int y, element_type element) { 

        switch(element) {
            case ELEMENT_NONE:
                return;
            case ELEMENT_BORDER: // should never happen
                return;
            case ELEMENT_SAND: 
                elements::element_sand_spawn(x, y);
                return;
            case ELEMENT_BRICK: 
                elements::element_brick_spawn(x, y);
                break;    
            case ELEMENT_WATER:
                elements::element_water_spawn(x, y);
                break;
            case ELEMENT_STEAM:
                elements::element_steam_spawn(x, y);
                break;
            case ELEMENT_FIRE:
                elements::element_fire_spawn(x, y);
                break;
            case ELEMENT_SALT:
                elements::element_salt_spawn(x, y);
                break;
            case ELEMENT_GUNPOWDER:
                elements::element_gunpowder_spawn(x, y);
                break;
            case ELEMENT_EXPLOSIVE_PARTCILE:
                elements::element_explosive_particle_spawn(x, y);
                break;
            case ELEMENT_INDESTRUCTIBLE_WALL:
                elements::element_indestructible_wall_spawn(x, y);
                break;
            case ELEMENT_WOOD:
                elements::element_wood_spawn(x, y);
                break;
            case ELEMENT_METAL:
                elements::element_metal_spawn(x, y);
                break;
            case ELEMENT_PETROLEUM:
                elements::element_petroleum_spawn(x, y);
                break;
            case ELEMENT_BUTANE:
                elements::element_butane_spawn(x, y);
                break;
            case ELEMENT_ACID:
                elements::element_acid_spawn(x, y);
                break;
            case ELEMENT_ACID_STEAM:
                elements::element_acid_steam_spawn(x, y);
                break;
            case ELEMENT_ICE:
                elements::element_ice_spawn(x, y);
                break;
        }
    }


}


#endif