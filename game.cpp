#include "game.hpp"

namespace game {
    
    void register_elements() {
        game::game_register_element(ELEMENT_NONE, "Erase", 4); // technically not an element

        game::game_register_element(ELEMENT_BRICK, "Brick", 35);
        game::game_register_element(ELEMENT_WOOD, "Wood", 39);
        game::game_register_element(ELEMENT_ICE, "Ice", 44);
        game::game_register_element(ELEMENT_METAL, "Metal", 42);
        game::game_register_element(ELEMENT_INDESTRUCTIBLE_WALL, "Indestructible wall", 38);

        game::game_register_element(ELEMENT_SAND, "Sand", 32);
        game::game_register_element(ELEMENT_WATER, "Water", 33);
        game::game_register_element(ELEMENT_SALT, "Salt", 36);

        game::game_register_element(ELEMENT_ACID, "Acid", 40);
        game::game_register_element(ELEMENT_PETROLEUM, "Petroleum", 43);
        game::game_register_element(ELEMENT_FIRE, "Fire", 34);
        game::game_register_element(ELEMENT_GUNPOWDER, "Gunpowder", 37);
        game::game_register_element(ELEMENT_BUTANE, "Butane", 41);
        
    }



    void game_init(sf::RenderWindow *window, int canvas_w, int canvas_h, int button_cols, int window_scaling, sf::Font &main_font) {
        if(canvas_w%2==1) {
            cout <<"error: canvas_w must be in mul of 2!!!"<<endl;
            exit(-1);
        }
        srand(time(NULL));

        last_draw_y = last_draw_x = -1;
        holding_on_canvas = false;
        benchmark_mode = false;

        #ifdef USE_EXTERNAL_RESOURCES
        if(!image_buttons.loadFromFile("resources/buttons.png")) {
            std::cout<<"failed to load resources/buttons.png"<<std::endl;
            exit(-1);
        }
        #else
        if(!image_buttons.loadFromMemory(res_buttons_png, res_buttons_png_size)) {
            std::cout<<"failed to load resources/buttons.png"<<std::endl;
            exit(-1);
        }
        #endif
        
        game_toast = new toast(main_font);
        game_toast->setMaxTransparency(128);

        w = canvas_w;
        h = canvas_h;
        scaling = window_scaling;

        // resize the window and view to game bounds
        window->setSize(sf::Vector2u(w*scaling, h*scaling + button_cols*49 + 2));

        window_w = w*scaling;
        window_h = h*scaling+ button_cols*49 + 2;
        sf::FloatRect visibleArea(0.f, 0.f, window_w, window_h);
        window->setView(sf::View(visibleArea));

        elements_buttons = new elements_container(button_cols, h*scaling+1, w*scaling);

        elements_buttons->setCallback(game_button_callback);

        // register buttons and then element
        game_register_button(BUTTON_EXIT, "Exit", 0);
        game_register_button(BUTTON_CLEAR, "Clear canvas", 5);
        game_register_button(BUTTON_PAUSE, "Pause", 1);
        game_register_button(BUTTON_TEMP_VIEW, "Temperature view", 6);
        game_register_button(BUTTON_BRUSH_MINUS, "Brush size -", 3);
        game_register_button(BUTTON_BRUSH_PLUS, "Brush size +", 2);
       
        register_elements();
        

        pixels = new pixel_array_entry[w*h];

        for(int i=0;i<w*h;i++) {
            pixels[i].id = ELEMENT_NONE;
            pixels[i].color.r = 0;
            pixels[i].color.g = 0;
            pixels[i].color.b = 0;
        }

        screen_draw = new uint8_t[w*h*4]; // the array on which we gonna draw
        for(int i=0;i<w*h*4;i=i+4) {
            screen_draw[i+3] = 255;
        }

        screen_texture = new sf::Texture();
        screen_texture->create(w, h);
        screen_texture->update(screen_draw);
        screen_sprite = new sf::Sprite(*screen_texture);

        screen_sprite->setPosition(0,0);
        screen_sprite->setScale(scaling,scaling);

        fps_text.setFont(main_font);
        fps_text.setCharacterSize(20);
        fps_text.setFillColor(sf::Color::White);
        fps_text.setPosition(0,0);
        fps_text.setString("FPS: 0");
        fps_clock.restart();
        fps_framecounter = 0;

        exit_on_next_frame = false;

        brush_id = ELEMENT_SAND;
        brush_size = 3;
        paused = false;
    }

    void game_register_element(element_type element, string name, int texture_id) {
        sf::Image copy_image;
        sf::Texture copied_button_texture;

        int x=texture_id%32;
        int y=texture_id/32;

        copy_image.create(48,48);
        copy_image.copy(image_buttons, 0, 0, sf::IntRect(x*48,y*48,48,48));

        copied_button_texture.loadFromImage(copy_image);
        
        elements_button_entry new_entry;
        new_entry.element = element;
        new_entry.button_name = name;
        new_entry.is_special_button = false;
        new_entry.id = elements_buttons_list.size()+1;
        elements_buttons_list.push_back(new_entry);

        elements_buttons->registerElement(new_entry.id, true, &copied_button_texture);
    }

    void game_register_button(special_button button, string name, int texture_id) {
        sf::Image copy_image;
        sf::Texture copied_button_texture;

        int x=texture_id%32;
        int y=texture_id/32;

        copy_image.create(48,48);
        copy_image.copy(image_buttons, 0, 0, sf::IntRect(x*48,y*48,48,48));

        copied_button_texture.loadFromImage(copy_image);
        
        elements_button_entry new_entry;
        new_entry.button = button;
        new_entry.button_name = name;
        new_entry.is_special_button = true;
        new_entry.id = elements_buttons_list.size()+1;
        elements_buttons_list.push_back(new_entry);

        elements_buttons->registerElement(new_entry.id, false, &copied_button_texture);
    }

    void physics_thread() {
        if(paused) return;
        for(int x=0;x<w;x=x+2) {
            for(int y=(h-1);y>=0;y--) {
                if(pixels[COORD(x,y)].physics_properties.type!=PHYSICS_GAS) {
                    elements::element_update(x,y, get_element_type(x,y));
                }
            }
            for(int y=0;y<h;y++) {
                if(pixels[COORD(x,y)].physics_properties.type==PHYSICS_GAS) {
                    elements::element_update(x,y, get_element_type(x,y));
                }
            }
        }

    }
    void physics_thread2() {
        if(paused) return;
        for(int x=w-1;x>=0;x=x-2) {
            for(int y=(h-1);y>=0;y--) {
                if(pixels[COORD(x,y)].physics_properties.type!=PHYSICS_GAS) {
                    elements::element_update(x,y, get_element_type(x,y));
                }
            }
            for(int y=0;y<h;y++) {
                if(pixels[COORD(x,y)].physics_properties.type==PHYSICS_GAS) {
                    elements::element_update(x,y, get_element_type(x,y));
                }
            }
        }
    }

    void game_tick(sf::RenderWindow *window) {
        // mouse update
        if(holding_on_canvas) {
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            int mouse_x = mousePos.x;
            int mouse_y = mousePos.y;

            line_draw(last_draw_x/scaling,last_draw_y/scaling,mouse_x/scaling,mouse_y/scaling);
            
            last_draw_x = mouse_x;
            last_draw_y = mouse_y;
        }

       
    }

    void game_show_toast(string message) {
        game_toast->showMessage(message, 3, window_w, window_h);
    }

    void game_process_event(sf::Event event, sf::RenderWindow *window) {

        if(event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            if(event.mouseButton.button==sf::Mouse::Left) {
                if(screen_sprite->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    holding_on_canvas = true;
                    int mouse_x = mousePos.x;
                    int mouse_y = mousePos.y;
                    
                    // draw 
                    pixel_draw(mouse_x/scaling,mouse_y/scaling);

                    last_draw_x = mouse_x;
                    last_draw_y = mouse_y;
                }
            }
            
        }

        if(event.type == sf::Event::MouseButtonReleased) {
            if(event.mouseButton.button==sf::Mouse::Left) {
                if(holding_on_canvas) {
                    last_draw_x = -1;
                    last_draw_y = -1;
                    holding_on_canvas = false;
                }
            }
        }

        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::F1) {
                if(brush_size>1) brush_size = brush_size - 1;
                game_show_toast("Brush size: " + to_string(brush_size));
            }
            if(event.key.code == sf::Keyboard::F2) {
                brush_size = brush_size + 1;
                game_show_toast("Brush size: " + to_string(brush_size));
            }

            if(event.key.code == sf::Keyboard::F9) {
                fps_display_enabled = !fps_display_enabled;
                game_show_toast(fps_display_enabled ? "FPS display ON" : "FPS display OFF");
            }
        }

        elements_buttons->processEvent(event, *window);
    }

    bool game_draw(sf::RenderWindow *window) {
        thread *phy = new thread(physics_thread) ; // we start physics thread here
        thread *phy2 = new thread(physics_thread2) ;
        if(benchmark_mode) goto skip_render;

        elements_buttons->draw(*window);

        if(temp_view) {
            for(int x=0;x<w;x++) {
                for(int y=0;y<h;y++) {
                    int i = y*w+x;
                    if(pixels[i].id != element_type::ELEMENT_NONE) {
                        int temp = pixels[i].physics_properties.temperature;
                        uint8_t r=0;
                        uint8_t g=0;
                        uint8_t b=0;

                        if (temp < 0) {
                            // Map temperature from 0 to -128 to blue range
                            b = 255 - (temp * -2);
                            if(b<4) b = 4; // to be visible
                        } else if (temp <= 40) {
                            // Black to Red
                            float ratio = float(temp) / 40.0f;
                            r = static_cast<uint8_t>(ratio * 255);
                            if(r<10) r = 10; // to be visible
                        } else if (temp <= 75) {
                            // Red to Yellow
                            float ratio = float(temp - 40) / 35.0f;
                            r = 255;
                            g = static_cast<uint8_t>(ratio * 255);
                        } else if (temp <= 127) {
                            // Yellow to White
                            float ratio = float(temp - 75) / 52.0f;
                            r = 255;
                            g = 255;
                            b = static_cast<uint8_t>(ratio * 255);
                        }



                        screen_draw[i*4+0] = r;
                        screen_draw[i*4+1] = g;
                        screen_draw[i*4+2] = b;
                        
                    } else {
                        screen_draw[i*4+0] = 0;
                        screen_draw[i*4+1] = 0;
                        screen_draw[i*4+2] = 0;
                    }
                }   
            }
        } else {
            for(int x=0;x<w;x++) {
                for(int y=0;y<h;y++) {
                    int i = y*w+x;
                    if(pixels[i].id != element_type::ELEMENT_NONE) {
                        screen_draw[i*4+0] = pixels[i].color.r;
                        screen_draw[i*4+1] = pixels[i].color.g;
                        screen_draw[i*4+2] = pixels[i].color.b;
                    } else {
                        screen_draw[i*4+0] = 0;
                        screen_draw[i*4+1] = 0;
                        screen_draw[i*4+2] = 0;
                    }
                }   
            }
        }
        screen_texture->update(screen_draw);

        window->draw(*screen_sprite);
        game_toast->draw(*window);

        skip_render:

        phy->join();
        phy2->join();
        delete phy;
        delete phy2;


        if(!paused) framecounter++;

        if(benchmark_mode) goto skip_post_render;

        fps_framecounter++;
        if(fps_clock.getElapsedTime().asMilliseconds()>1000) {
            fps_text.setString("FPS: " + to_string(fps_framecounter));
            fps_framecounter = 0;
            fps_clock.restart();
        }
        if(fps_display_enabled) window->draw(fps_text);

        if(exit_on_next_frame) {
            game_exit();
            return false;
        }
        if(clear_on_next_frame) {
            for(int i=0;i<w*h;i++) {
                pixels[i].id = ELEMENT_NONE;
                pixels[i].color.r = 0;
                pixels[i].color.g = 0;
                pixels[i].color.b = 0;
            }
            clear_on_next_frame = false;
        }
        skip_post_render:
        return true;
    }


    void game_button_callback(int b_id) {
        // get clicked element
        for(unsigned int i=0;i<elements_buttons_list.size();i++) {
            if(b_id==elements_buttons_list[i].id) {
                if(elements_buttons_list[i].is_special_button) {
                    special_button button = elements_buttons_list[i].button;
                    // settings buttons
                    if(button == BUTTON_EXIT) {   
                        if(exit_double_click_clock.getElapsedTime().asMilliseconds()<500) {
                            exit_on_next_frame = true;
                        } else {
                            exit_double_click_clock.restart();
                            game_show_toast("double click to exit");
                        }
                        
                    }
                    if(button == BUTTON_CLEAR) {   
                        if(clear_double_click_clock.getElapsedTime().asMilliseconds()<500) {
                            clear_on_next_frame = true;
                        } else {
                            clear_double_click_clock.restart();
                            game_show_toast("double click to clear");
                        }
                        
                    }
                    if(button == BUTTON_PAUSE) {
                        paused = !paused;
                        game_show_toast(paused ? "Paused" : "Unpaused");
                    }
                    if(button == BUTTON_TEMP_VIEW) {
                        temp_view = !temp_view;
                        game_show_toast(temp_view ? "Temperature view" : "Normal view");
                    }
                    if(button == BUTTON_BRUSH_PLUS) {
                        brush_size++;
                        game_show_toast("Brush size: " + to_string(brush_size));
                    }
                    if(button == BUTTON_BRUSH_MINUS) {
                        if(brush_size>1) brush_size--;
                        game_show_toast("Brush size: " + to_string(brush_size));
                    }

                } else {
                    // element button
                    // set brush to element
                    brush_id = elements_buttons_list[i].element;
                    game_toast->showMessage(elements_buttons_list[i].button_name, 3, window_w, window_h);
                }

            }
        }
    }


    void pixel_draw(int x, int y) {
        
        int minus = 0;
        if(brush_size>1) minus = (brush_size - 1) / 2;

        for(int xx=0;xx<brush_size;xx++) {
            for(int yy=0;yy<brush_size;yy++) {
                // check if in bounds
                int dx = x+xx-minus;
                int dy = y+yy-minus;
                if(dx<0 || dx>=w || dy<0 || dy>=h) continue;
                // put here pixel spawn code
                //int i = dy*w+dx;
                //pixels[i].color.r = 0;
                
                // if ELEMENT_NONE, erase the pixel
                if(brush_id==ELEMENT_NONE) {
                    physics::clear_pixel(dx, dy);
                } else {
                    elements::element_spawn(dx, dy, brush_id);
                }
                
            }   
        }
    }

    void line_draw(int x1, int y1, int x2, int y2) { // https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm
        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
        dx = x2 - x1;
        dy = y2 - y1;
        dx1 = fabs(dx);
        dy1 = fabs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;
        if (dy1 <= dx1) {
            if (dx >= 0) {
                x = x1;
                y = y1;
                xe = x2;
            } else {
                x = x2;
                y = y2;
                xe = x1;
            }
            pixel_draw(x,y);
            for (i = 0; x < xe; i++) {
                x = x + 1;
                if (px < 0) {
                    px = px + 2 * dy1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        y = y + 1;
                    } else {
                        y = y - 1;
                    }
                    px = px + 2 * (dy1 - dx1);
                }
                pixel_draw(x,y);
            }
        } else {
            if (dy >= 0) {
                x = x1;
                y = y1;
                ye = y2;
            } else {
                x = x2;
                y = y2;
                ye = y1;
            }
            pixel_draw(x,y);
            for (i = 0; y < ye; i++) {
                y = y + 1;
                if (py <= 0) {
                    py = py + 2 * dx1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        x = x + 1;
                    } else {
                        x = x - 1;
                    }
                    py = py + 2 * (dx1 - dy1);
                }
                pixel_draw(x,y);
            }
        }
    }

    enum element_type get_element_type(int x, int y) {
        if(x<0 || x>=game::w || y<0 || y>=game::h) {
            if(borders_enabled) return ELEMENT_BORDER;
            return ELEMENT_NONE;
        }
        
        return game::pixels[COORD(x,y)].id;
    }

    bool rng_bool() {
        if(benchmark_mode) return true;
        return rand() > (RAND_MAX / 2);
    }
    int rng_int(int min, int max) {
        if(benchmark_mode) return min;
        return (rand() % (max - min + 1)) + min;
    }
    
    void game_exit() {
        // DEINITIALIZE

        
        delete game_toast;
        delete elements_buttons;
        delete pixels;
        delete screen_draw;
        delete screen_sprite;
        delete screen_texture;
        
        elements_buttons_list.clear();
    }

    /*
        Benchmark to test compiler optimalizations
        For benchmark mode we also break randomizer to make sure to have same results
    */
    int game_benchmark() { // has own init
        benchmark_mode = true;
        w = 210;
        h = 250;
        borders_enabled = true;
        paused = false;

        pixels = new pixel_array_entry[w*h];

        for(int i=0;i<w*h;i++) {
            pixels[i].id = ELEMENT_NONE;
        }
        // how test works: fill screen with single material and check how much one frame takes
        // sand
        for(int x=0;x<w;x++) {
            for(int y=0;y<h;y++) {
                elements::element_spawn(x,y,ELEMENT_FIRE);
            }
        }
        sf::Clock start_time;
        uint32_t ms, lowest;
        lowest = 99999999;
        for(int z=0;z<120;z++) {
            start_time.restart();
            for(int i=0;i<30;i++) {
                physics_thread();
                physics_thread2();
            }
            ms = start_time.getElapsedTime().asMicroseconds();
            if(ms<lowest) lowest = ms;
        }
        
        //cout<<"LOWEST MS: "<<lowest<<endl;

        delete pixels;
        return lowest;
    }
};