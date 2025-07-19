#ifndef UI_TOAST_HPP
#define UI_TOAST_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "roundedrectangleshape.hpp"
#include <iostream>
#include <stdint.h>

class toast {
public:
    toast(const sf::Font& font);
    
    void draw(sf::RenderWindow& window);

    void showMessage(std::string message, int duration_sec, int window_w, int window_h);
    
    void setMaxTransparency(uint8_t max_transparency);

private:
    sf::Text toast_text;
    sf::RoundedRectangleShape rrs_background;
    sf::Clock timer;
    int duration;
    int elapsed_time_offset;
    bool showing;
    uint8_t max_transparency;
};

#endif // UI_ELEMENTS_HPP