#include "toast.hpp"

toast::toast(const sf::Font& font) {
    toast_text.setFont(font);
    toast_text.setCharacterSize(20);
    showing = false;
    max_transparency = 255;
}

void toast::showMessage(std::string message, int duration_sec, int window_w, int window_h) {
    toast_text.setString(message);
    sf::FloatRect textBounds = toast_text.getLocalBounds();

    toast_text.setOrigin(textBounds.left + textBounds.width / 2.0f, 0);
    toast_text.setPosition(window_w / 2, window_h - 103);

    toast_text.setFillColor(sf::Color(255,255,255,0));
    rrs_background.setFillColor(sf::Color(96,96,96,0));

    rrs_background.setSize(sf::Vector2f(textBounds.width + 40, textBounds.height + 15));
    rrs_background.setPosition(window_w / 2 - (textBounds.width + 40)/2,  window_h - 105);
    rrs_background.setCornersRadius(10);
    rrs_background.setCornerPointCount(6);
    
    if(duration_sec<2) duration_sec = 2; // minimum 2 sec (1 sec for fade in and out)
    duration = duration_sec;
    timer.restart();
    // if toast was still showing then add 250 to time
    if(showing) {
        elapsed_time_offset = 400;
    } else {
        elapsed_time_offset = 0;
    }
    showing = true;
}


void toast::draw(sf::RenderWindow& window) {
    if(!showing) return;

    sf::Time elapsed_time = timer.getElapsedTime();
    long int elapsed_ms = elapsed_time.asMilliseconds() + elapsed_time_offset;

    if(elapsed_ms<500) { 
        // fade in
        int fade_amount = elapsed_ms / (float)(500/(float)255);
        if(fade_amount>=254) fade_amount = 255;
        int fade_amount_bg = fade_amount;
        if(fade_amount_bg>max_transparency) fade_amount_bg = max_transparency;

        toast_text.setFillColor(sf::Color(255,255,255,fade_amount));
        rrs_background.setFillColor(sf::Color(96,96,96,fade_amount_bg));
    }

    if(elapsed_ms>(duration - 1) *1000) { 
        // fade out
        int fade_amount = ( duration*1000 - elapsed_ms ) / (float)(1000/(float)255);
        

        if(fade_amount<=1) {
            fade_amount = 0;
            showing = false;
        }
        
        int fade_amount_bg = fade_amount;
        if(fade_amount_bg>max_transparency) fade_amount_bg = max_transparency;

        toast_text.setFillColor(sf::Color(255,255,255,fade_amount));
        rrs_background.setFillColor(sf::Color(96,96,96,fade_amount_bg));
    }
    
    window.draw(rrs_background);
    window.draw(toast_text);
}

void toast::setMaxTransparency(uint8_t max_transparency) {
    this->max_transparency = max_transparency;
}