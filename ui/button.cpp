#include "button.hpp"

button::button(const std::string& text, const sf::Font& font, unsigned int characterSize, int x, int y, int w, int h) {
    callback = nullptr;
    sf::Vector2f position(x, y);
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(characterSize);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = buttonText.getLocalBounds();

    buttonText.setPosition(position.x + w/2 - textBounds.width/2, position.y + h/2 - textBounds.height/2 - 4);

    
    buttonShape.setSize(sf::Vector2f(w, h));
    buttonShape.setPosition(position.x, position.y);
    buttonShape.setFillColor(sf::Color(32,48,96));
}

void button::processEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonReleased&&heldOnButton) {
        if (event.mouseButton.button == sf::Mouse::Left && isHovered(window)) {
            if(callback) callback();
        }
        buttonShape.setFillColor(sf::Color(32,48,96));    
        heldOnButton = false;
        wasLastHovered = false;
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && isHovered(window)) {
            heldOnButton = true;
            buttonShape.setFillColor(sf::Color(170,170,188));
        }
    }
}

void button::draw(sf::RenderWindow& window) {
    if(!heldOnButton && window.hasFocus()) {
        if(isHovered(window)) {
            if(wasLastHovered!=false) {
                buttonShape.setFillColor(sf::Color(70,69,85));
                wasLastHovered = false;
            }
        } else {
            if(wasLastHovered!=true) {
                buttonShape.setFillColor(sf::Color(32,48,96));
                wasLastHovered = true;
            }
        }
    }
    window.draw(buttonShape);
    window.draw(buttonText);
}

void button::setCallback(Callback callback) {
    this->callback = callback;
}

bool button::isHovered(const sf::RenderWindow& window) const {
    sf::Vector2i mousePosGlobal = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePosGlobal);
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}