#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class elements_container {
public:
    using Callback = void (*)(int);

    elements_container(int cols, int y, int w);

    void processEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void setCallback(Callback callback);
    void registerElement(int element_id, bool selectable, sf::Texture *button_image);

private:
    int y,w;

    Callback callback;
    bool isButtonClicked(const sf::Vector2f& mousePos, int& button_id, int& button_index) ;

    struct elements_entry_struct {
        int id;
        bool selectable;
        sf::Texture texture;
    };

    vector<elements_entry_struct> elements;

    int cols;
    int scroll_x = 0;
    int max_scroll_x=0;

    int mouse_click_x, mouse_click_y;
    int last_scroll_x, last_mouse_x;
    int selected_button;
    int hover_button;
    int scroll_accel;
    bool holding=false;
    bool mouse_not_moved_too_far=false;
    sf::Clock click_duration;

    sf::RectangleShape fr_button_background;
    sf::RectangleShape rs_selected_background;
    sf::RectangleShape rs_hover_background;
    sf::Sprite button_drawer;

};

#endif // UI_ELEMENTS_HPP