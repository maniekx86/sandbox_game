#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class button {
public:
    using Callback = void (*)();

    button(const std::string& text, const sf::Font& font, unsigned int characterSize, int x,int y, int w,int h);

    void processEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void setCallback(Callback callback);
    

private:
    sf::Text buttonText;
    sf::RectangleShape buttonShape;
    Callback callback;
    bool isHovered(const sf::RenderWindow& window) const;
    bool heldOnButton=false;
    bool wasLastHovered=false;
};

#endif // UI_BUTTON_HPP