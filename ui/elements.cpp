#include "elements.hpp"

elements_container::elements_container(int cols, int y, int w) {
    callback = nullptr;

    this->y = y;
    this->w = w;
    this->cols = cols;
    
    mouse_click_x = -1;
    mouse_click_y = -1;
    scroll_accel = 0;
    selected_button = -1;
    hover_button = -1;

    fr_button_background.setPosition(0, y-1);
    fr_button_background.setSize(sf::Vector2f(w, cols*49 + 2));
    fr_button_background.setFillColor(sf::Color(128,128,128));
    
    rs_selected_background.setSize(sf::Vector2f(48,48));
    rs_selected_background.setOutlineColor(sf::Color(192,0,0,192));
    rs_selected_background.setOutlineThickness(-3);
    rs_selected_background.setFillColor(sf::Color(128,0,0,64));

    rs_hover_background.setSize(sf::Vector2f(48,48));
    rs_hover_background.setFillColor(sf::Color(96,0,0,64));
}

void elements_container::registerElement(int element_id, bool selectable, sf::Texture *button_image) {
    elements_entry_struct new_element;
    
    new_element.id = element_id;
    new_element.selectable = selectable;
    // copy the texture
    new_element.texture = *button_image;

    elements.push_back(new_element);

    // compute max_scroll_x
    int elements_width = ceil(elements.size()/(float)cols) * 49;
    max_scroll_x = elements_width - w;
    if(max_scroll_x<0) max_scroll_x = 1;
}

void elements_container::processEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int mouse_x = mousePos.x;
    int mouse_y = mousePos.y;

    /*
        drag vs click:
        click:
        position between click and release not large
        time holding: any
        drag:
        while holding, position between click and release large
    
    */

    if(window.hasFocus()&&fr_button_background.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        if (event.type == sf::Event::MouseButtonPressed) {
            mouse_click_x = mouse_x;
            mouse_click_y = mouse_y;
            last_scroll_x = scroll_x;
            click_duration.restart();
            holding = true;
            scroll_accel = 0;
            mouse_not_moved_too_far = true;

            int button_id, button_index;
            if (isButtonClicked(mousePos, button_id, button_index)) {
                hover_button = button_index;
            } else {
                hover_button = -1;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (holding && mouse_not_moved_too_far) {
                // Check if a button was clicked
                int button_id, button_index;
                if (isButtonClicked(mousePos, button_id, button_index)) {
                    if(elements[button_index].selectable) {
                        selected_button = button_index;
                    }
                    if (callback) {
                        callback(button_id);   
                    }
                }
            }

            scroll_accel = (mouse_x - last_mouse_x); 
            holding = false;
        }

        if(event.type==sf::Event::MouseWheelMoved) {
            scroll_x=(scroll_x/48)*49;
            scroll_x=scroll_x-event.mouseWheel.delta*49;
            if(scroll_x<0) scroll_x = 0;
            if(scroll_x>=max_scroll_x) scroll_x = max_scroll_x-1;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) { // if released outside our bounds
        if(holding) scroll_accel = (mouse_x - last_mouse_x); 
        holding = false;
    }
}

void elements_container::draw(sf::RenderWindow& window) {

    if(holding) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        int mouse_x = mousePos.x;
        int mouse_y = mousePos.y;

        scroll_x = last_scroll_x + (mouse_click_x - mouse_x);
        if(scroll_x<0) scroll_x = 0;
        if(scroll_x>=max_scroll_x) scroll_x = max_scroll_x-1;

        if(!(std::abs(mouse_click_x - mouse_x) < 5 && std::abs(mouse_click_y - mouse_y) < 5)) {
            mouse_not_moved_too_far = false;
            hover_button = -1;
        }

        last_mouse_x = mouse_x;
    } else {
        if(scroll_accel>1) {
            scroll_x = scroll_x - scroll_accel;
            scroll_accel--;
            if(scroll_x<0) { // stop if end
                scroll_x = 0;
                scroll_accel = 0;
            }
        }
        if(scroll_accel<-1) {
            scroll_x = scroll_x - scroll_accel;
            scroll_accel++;
            if(scroll_x>=max_scroll_x) {
                scroll_x = max_scroll_x-1;
                scroll_accel = 0;
            }
        }
    }
    
    window.draw(fr_button_background);

    int my_x=0;
    int my_col=0;

    for(unsigned int i=0; i<elements.size();i++) {
        // skip drawing if outside
        // left
        if(my_x-scroll_x<-48) goto skip_drawing;
        // right
        if(my_x-scroll_x>w) goto skip_drawing;

        button_drawer.setTexture(elements[i].texture);
        button_drawer.setPosition(my_x-scroll_x, y+1+my_col*49);
        //button_drawer.setPosition(0,0);

        window.draw(button_drawer);
        if(selected_button==(signed int)i) {
            rs_selected_background.setPosition(my_x-scroll_x, y+1+my_col*49);
            window.draw(rs_selected_background);
        } else {

            if(hover_button==(signed int)i && holding && click_duration.getElapsedTime().asMilliseconds()>100) { // if also holding for more than some time
                
                rs_hover_background.setPosition(my_x-scroll_x, y+1+my_col*49);
                window.draw(rs_hover_background);
            }
        }
        

        skip_drawing:

        my_col++;
        if(my_col>=cols) {
            my_col = 0;
            my_x = my_x + 49;
        }
    }
}

void elements_container::setCallback(Callback callback) {
    this->callback = callback;
}

bool elements_container::isButtonClicked(const sf::Vector2f& mousePos, int& button_id, int& button_index) {
    int my_x = 0;
    int my_col = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        sf::FloatRect buttonBounds(my_x - scroll_x, y + 1 + my_col * 49, 49, 49);
        
        if (buttonBounds.contains(mousePos)) {
            button_id = elements[i].id;
            button_index = i;
            return true;
        }

        my_col++;
        if (my_col >= cols) {
            my_col = 0;
            my_x += 49;
        }
    }

    return false;
}