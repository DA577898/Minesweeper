//
// Created by Daniel on 4/18/2024.
//
#include <string>
#include "SFML/Graphics.hpp"
#include "GameSettings.h"
#include "MinesweeperCore.h"
#include "GameScreen.h"
#include "LeaderboardScreen.h"
#include "WindowUtils.h"
#include <iostream>

void GameScreen(Minesweeper& game){
    removeAsterisks();

    auto* game_window = new sf::RenderWindow(sf::VideoMode(game.getSettings().getGameScreenDimensions().x, game.getSettings().getGameScreenDimensions().y), "Minesweeper", sf::Style::Close);
    bool winScreenHasShown = false;
    int previous_time = 0;
    AssetManager assets(true);

    RenderGame(game_window, game, assets);

    while(game_window->isOpen()) {
        sf::Event event;
        while(game_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game_window->close();
                return;
            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2<int> pos = sf::Mouse::getPosition(*game_window);
                string clicked_button = checkButtonCollision(game, pos, event);
                if(game.getGameStatus() == "happy"){ // buttons that can be clicked if game is not lost
                    // Buttons that can be pressed only if game is not lost and not paused
                    if(!game.getIsPaused()) {
                        if (clicked_button == "debug") game.toggleDebugMode();
                        else if (clicked_button.substr(0, 9) == "tile_left") game.discoverTile(tile_coordinates(clicked_button).x, tile_coordinates(clicked_button).y);
                        else if (clicked_button.substr(0, 10) == "tile_right") game.flagTile(tile_coordinates(clicked_button).x, tile_coordinates(clicked_button).y);
                    }
                    // Buttons that can be pressed only if game is not lost
                    if (clicked_button == "pause/play") game.pause();
                }

                // Buttons that can be pressed regardless
                if (clicked_button == "face") {
                    game.GenerateBoard();
                    winScreenHasShown = false;
                } else if (clicked_button == "leaderboard") {
                    game.pauseTimer();
                    RenderGame(game_window, game, assets, game.getGameStatus() == "happy"); // last statement allows players to see the current game in leaderboard if they won/lost
                    Leaderboard(game);
                    while (game_window->pollEvent(event)); // Wipe out any events that occurred while the leaderboard was open
                    if (!game.getIsPaused() & game.getGameStatus() == "happy") { game.resumeTimer(); } // Once they're back, resume game if it wasn't paused.
                }

                // After all the button checks, check if the player won AND the leaderboard hasn't shown yet

                if(game.getGameStatus() == "win" & !winScreenHasShown){
                    game.pauseTimer();
                    addPlayerToLeaderboard(game.getElapsedTime(), game.getUsername());
                    RenderGame(game_window, game, assets);
                    Leaderboard(game);
                    while (game_window->pollEvent(event));
                    winScreenHasShown = true;
                }
                // Render the game after the event.
                RenderGame(game_window, game, assets);
            }
        }
        if(game.getElapsedTime() != previous_time) { // Timer event, only refreshes the screen if time has passed.
            RenderGame(game_window, game, assets);
            previous_time = game.getElapsedTime();
        }
    }
}

