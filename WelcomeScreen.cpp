//
// Created by Daniel on 4/18/2024.
//

#include <string>
#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include "MinesweeperCore.h"
#include "WelcomeScreen.h"
#include "WindowUtils.h"

string WelcomeScreen(GameSettings settings){
    float width = settings.getWelcomeScreenDimensions().x;
    float height = settings.getWelcomeScreenDimensions().y;
    string username = "";
    auto* welcome_window = new sf::RenderWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    welcome_window->clear(sf::Color::Blue);
    DrawText(welcome_window, "WELCOME TO MINESWEEPER!", width/2, (height/2)-150, 24, true, true, sf::Color::White);
    DrawText(welcome_window, "Enter your name:", width/2, (height/2)-75, 20, true, false, sf::Color::White);
    DrawText(welcome_window, username + "|", width/2, (height/2)-45, 20, true, false, sf::Color::White);
    welcome_window->display();

    while(welcome_window->isOpen()) {
        sf::Event event;
        while(welcome_window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcome_window->close();
                return ""; // Since it's not possible to return an empty string, this closes the program instantly.
            }

            if(event.type == sf::Event::KeyPressed) {
                char letter = getLetter(event.key.code);
                if (event.key.code == 59){if(!username.empty()){username = username.substr(0, username.length()-1);}} // backspace handler
                if(letter != ' ' && username.length() < 10 ){ // letter handler
                    if(username.length() == 0) {letter = toupper(letter);}
                    username += letter;
                }
                if (event.key.code == 58 && username.length() != 0){welcome_window->close();return username;} // enter key handler

                welcome_window->clear(sf::Color::Blue);
                DrawText(welcome_window, "WELCOME TO MINESWEEPER!", width/2, (height/2)-150, 24, true, true, sf::Color::White);
                DrawText(welcome_window, "Enter your name:", width/2, (height/2)-75, 20, true, false, sf::Color::White);
                DrawText(welcome_window, username + "|", width/2, (height/2)-45, 20, true, false, sf::Color::White);
                welcome_window->display();
            }
        }
    }
    return "";
}
