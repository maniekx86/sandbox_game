#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <thread>
#include "physics/physics.hpp"
#include "physics/common.hpp"
#include "elements/elements.hpp"
#include "ui/elements.hpp"
#include "ui/toast.hpp"

#define COORD(x, y) ((y) * game::w + (x))

#ifndef USE_EXTERNAL_RESOURCES
extern const char res_buttons_png[];
extern const unsigned res_buttons_png_size;
#endif

namespace game {

    void game_init(sf::RenderWindow *window, int canvas_w, int canvas_h, int button_rows, int window_scaling, sf::Font &main_font);
    void register_elements();
    void game_tick(sf::RenderWindow *window);
    void game_process_event(sf::Event event, sf::RenderWindow *window);
    bool game_draw(sf::RenderWindow *window);
    void game_register_element(element_type element, string name, int texture_id);
    void game_exit();
    void game_show_toast(string message);
    int game_benchmark();

    void game_button_callback(int b_id);
    
    void physics_thread();
    void physics_thread2();

    void pixel_draw(int x, int y);
    void line_draw(int xa, int ya, int xb, int yb);

    bool rng_bool();
    int rng_int(int min, int max);

    enum element_type get_element_type(int x, int y);

    inline int window_w, window_h;
    inline int w, h, scaling;
    inline int brush_size;

    inline element_type brush_id;

    inline bool benchmark_mode=false; // benchmark for testing compiler optimalization
    inline bool paused;
    inline bool temp_view;

    inline int last_draw_x, last_draw_y;
    inline bool holding_on_canvas;

    inline sf::Image image_buttons;

    inline elements_container *elements_buttons;

    enum special_button {
        BUTTON_EXIT,
        BUTTON_PAUSE,
        BUTTON_BRUSH_PLUS,
        BUTTON_BRUSH_MINUS,
        BUTTON_SETTINGS,
        BUTTON_CLEAR,
        BUTTON_TEMP_VIEW,
    };

    void game_register_button(special_button button, string name, int texture_id);

    struct elements_button_entry {
        int id;
        bool is_special_button;
        special_button button;
        element_type element;
        string button_name;
    };

    inline vector<elements_button_entry> elements_buttons_list;

    struct pixel_color { // sf::Color includes alpha which we dont need
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    

    struct pixel_array_entry {
        enum element_type id;
        pixel_color color;

        physics::properties physics_properties;

        uint8_t custom_properties[16]; // 16 bytes for element specific properties
    };

    inline pixel_array_entry *pixels;

    inline sf::Clock fps_clock;
    inline sf::Text fps_text;
    inline int fps_framecounter;
    inline bool fps_display_enabled = true;

    inline uint8_t *screen_draw;
    inline sf::Texture *screen_texture;
    inline sf::Sprite *screen_sprite;
    inline toast *game_toast;

    inline int framecounter;

    inline bool exit_on_next_frame;
    inline sf::Clock exit_double_click_clock;
    inline bool clear_on_next_frame;
    inline sf::Clock clear_double_click_clock;

    inline bool borders_enabled = true;
};


#endif


