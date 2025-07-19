#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "config.hpp"
#include "ui/button.hpp"
#include "game.hpp"
#include "syscross.hpp"
#include "ui/toast.hpp"

using namespace std;

sf::Font font_default;

sf::RenderWindow *window;

enum gameState_enum {
    menu_main,
    menu_settings,
    menu_credits,
    game_exit,
    game_main
} gameState;

toast *menu_toast;

///// buttons callbacks
// main menu
void exit_button_click() { // Exit
    gameState = game_exit;
}
void start_button_click() { // Start
    gameState = game_main;
    game::game_init(window, 420/2, 500/2, 2, 2, font_default);
}
void settings_button_click() { // Settings
    int result = game::game_benchmark();
    menu_toast->showMessage("bench result: " + to_string(result) + " ms", 5, 420, 600);
    
}
void credits_button_click() { // Credits
    gameState = menu_credits;
}
// credits menu
void credits_button_back_click() { // Back
    gameState = menu_main;
}
void credits_button_website_click() { // Website
    syscross::openURL(CONFIG_WEBSITE_ADDR);
}
void credits_button_changelog_click() { // Changelog
    syscross::openURL(CONFIG_CHANGELOG_ADDR);
}

// embeded resources

#ifndef USE_EXTERNAL_RESOURCES
extern const char res_font[];
extern const unsigned res_font_size;

extern const char res_logo_png[];
extern const unsigned res_logo_png_size;
#endif

int main() {
    #ifdef USE_EXTERNAL_RESOURCES
    if (!font_default.loadFromFile("resources/joystix.monospace-regular.otf")) {
        cout<<"Can't load font"<<endl;
        return -1;
    }
    #else 
    if (!font_default.loadFromMemory(res_font,res_font_size)) {
        cout<<"Can't load font"<<endl;
        return -1;
    }
    #endif

    gameState = menu_main;
    
    window = new sf::RenderWindow(sf::VideoMode(420,600), "Sand:Box PC " CONFIG_VER_STR);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);


    // main menu elements
    button start_game_button("Start game", font_default, 20, 110, 260, 200, 50);
    button settings_button("Settings", font_default, 20, 110, 340, 200, 50);
    button authors_button("Credits", font_default, 20, 110, 420, 200, 50);
    button exit_button("Exit", font_default, 20, 110, 500, 200, 50);

    start_game_button.setCallback(start_button_click);
    settings_button.setCallback(settings_button_click);
    authors_button.setCallback(credits_button_click);
    exit_button.setCallback(exit_button_click);

    // bottom text
    sf::Text text_footer("\nSand:Box PC " CONFIG_VER_STR " | Copyright 2024 (c) maniek86", font_default, 10);
    text_footer.setPosition(sf::Vector2f(5,568));

    // logo
    sf::Texture texture_img_logo;
    // i don't consider missing logo as something to make game exit
    #ifdef USE_EXTERNAL_RESOURCES
    if(!texture_img_logo.loadFromFile("resources/logo2.png")) {
        cout<<"Can't load logo2.png"<<endl;
    }
    #else
    if(!texture_img_logo.loadFromMemory(res_logo_png, res_logo_png_size)) {
        cout<<"Can't load logo2.png"<<endl;
    }   
    #endif
    sf::Sprite sprite_img_logo;
    sprite_img_logo.setTexture(texture_img_logo, true);
    sprite_img_logo.setPosition(0,0);
    sprite_img_logo.setScale(2,2);

    // credits elements
    button credits_back_button("Back", font_default, 20, 110, 530, 200, 50);
    button credits_website_button("Website", font_default, 20, 20, 460, 180, 50);
    button credits_changelog_button("Changelog", font_default, 20, 220, 460, 180, 50);
    sf::Text text_credits(CONFIG_CREDITS_STR, font_default, 20);
    sf::Sprite sprite_img_logo_credits;

    text_credits.setOrigin(text_credits.getGlobalBounds().width / 2.f + text_credits.getLocalBounds().left, 0);
    text_credits.setPosition(420 / 2, 24);

    sprite_img_logo_credits.setTexture(texture_img_logo, true);
    sprite_img_logo_credits.setPosition(105, text_credits.getGlobalBounds().top+text_credits.getGlobalBounds().height+30);

    credits_back_button.setCallback(credits_button_back_click);
    credits_website_button.setCallback(credits_button_website_click);
    credits_changelog_button.setCallback(credits_button_changelog_click);

    menu_toast = new toast(font_default);
    


    while (window->isOpen()) {
        sf::Event event;

        if(gameState == menu_main) {    
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) window->close();


                start_game_button.processEvent(event, *window);
                settings_button.processEvent(event, *window);
                authors_button.processEvent(event, *window);
                exit_button.processEvent(event, *window);

            }

            window->clear();


            start_game_button.draw(*window);
            settings_button.draw(*window);
            authors_button.draw(*window);
            exit_button.draw(*window);

            menu_toast->draw(*window);

            window->draw(text_footer);
            window->draw(sprite_img_logo);

            window->display();
            continue;
        }

        if(gameState == menu_credits) {
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) window->close();

                credits_back_button.processEvent(event, *window);
                credits_website_button.processEvent(event, *window);
                credits_changelog_button.processEvent(event, *window);
            }

            window->clear();

            window->draw(text_credits);
            window->draw(sprite_img_logo_credits);

            credits_back_button.draw(*window);
            credits_website_button.draw(*window);
            credits_changelog_button.draw(*window);

            menu_toast->draw(*window);

            window->display();
            continue;
        }


        if(gameState == game_exit) {
            while (window->pollEvent(event)) {
            }
            window->close();
        }

        if(gameState == game_main) {
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) window->close();
                game::game_process_event(event, window);
            }

            game::game_tick(window);
            if(!game::game_draw(window)) {
                // game exited
                gameState = menu_main;
                // reset size
                window->setSize(sf::Vector2u(420, 600));
            
                sf::FloatRect visibleArea(0.f, 0.f, 420, 600);
                window->setView(sf::View(visibleArea));

            } else {
                window->display();
            }

            continue;
        }
        
    }

    return 0;
}