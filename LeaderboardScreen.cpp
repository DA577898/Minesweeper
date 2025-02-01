//
// Created by Daniel on 4/18/2024.
//

#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "GameSettings.h"
#include "MinesweeperCore.h"
#include "WindowUtils.h"
#include "LeaderboardScreen.h"

bool Leaderboard(Minesweeper& game){
    float width = game.getSettings().getLeaderboardScreenDimensions().x;
    float height = game.getSettings().getLeaderboardScreenDimensions().y;
    auto* leaderboard_window = new sf::RenderWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    leaderboard_window->clear(sf::Color::Blue);
    DrawText(leaderboard_window, "LEADERBOARD", width/2, (height/2)-120, 24, true, true, sf::Color::White);
    DrawText(leaderboard_window, getLeaderboardPlayers(), width/2, (height/2)+20, 20, true, false, sf::Color::White);
    leaderboard_window->display();

    while(leaderboard_window->isOpen()) {
        sf::Event event;
        while(leaderboard_window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                leaderboard_window->close();
                return false;
            }
        }
    }
    return false;
}

