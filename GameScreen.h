//
// Created by Daniel on 4/18/2024.
//

#ifndef PROJECT3_GAMESCREEN_H
#define PROJECT3_GAMESCREEN_H
#pragma once

#include <sstream>
#include "WindowUtils.h"

void GameScreen(Minesweeper& game);

static void render_buttons(sf::RenderWindow* game_window, Minesweeper game, AssetManager& assets){
    DrawSprite(game_window, "face_" + game.getGameStatus(), (((game.getNumColumns()*32.0)/2.0) - 32), (32 * (game.getNumRows()+0.5)), assets);
    DrawSprite(game_window, "debug", (game.getNumColumns()*32.0) - 304, 32 * (game.getNumRows()+0.5), assets);
    string pause; if (game.getIsPaused() & game.getGameStatus() == "happy"){pause = "play";} else {pause = "pause";}
    DrawSprite(game_window, pause, (game.getNumColumns()*32.0) - 240, 32 * (game.getNumRows()+0.5), assets);
    DrawSprite(game_window, "leaderboard", (game.getNumColumns()*32.0) - 176, 32 * (game.getNumRows()+0.5), assets);
}

static void render_cells(sf::RenderWindow* game_window, Minesweeper game, AssetManager& assets, bool isInLeaderboard = false){
    for (int i = 0; i < game.getNumRows(); i++) {
        for (int j = 0; j < game.getNumColumns(); j++) {
            // First, draw the tiles.
            if ((game.getIsPaused() || isInLeaderboard) || game.getIsDiscovered(i, j)) {
                DrawSprite(game_window, "tile_revealed", j*32, i*32, assets);
            }
            else {
                DrawSprite(game_window, "tile_hidden", j*32, i*32, assets);
            }
            // Check. If the game is not paused, or the player won/lost, it can render elements
            if(!game.getIsPaused() || game.getGameStatus() != "happy"){
                // First, draw the flags.
                if(game.getIsFlagged(i,j)){
                    DrawSprite(game_window, "flag", j*32, i*32, assets);
                }
                // Second, draw the numbers.
                if(game.getCell(i, j) > 0 & game.getIsDiscovered(i, j) & !(game.getIsPaused() || isInLeaderboard)) {
                    DrawSprite(game_window, "number_" + to_string(game.getCell(i, j)), j * 32, i * 32, assets);
                }
                // Lastly, draw the mines, or flags if the player won.
                if(game.getIsMine(i, j) & (game.getIsDebugMode() || game.getGameStatus() == "lose")){
                    DrawSprite(game_window, "mine", j*32, i*32, assets);
                }
            }
        }
    }
}

static sf::Vector2<int> tile_coordinates(string tile_coordinates){
    sf::Vector2<int> coords;
    stringstream ss(tile_coordinates);
    string token;
    getline(ss, token, ',');
    getline(ss, token, ',');
    coords.x = stoi(token);
    getline(ss, token, ',');
    coords.y = stoi(token);
    return coords;
}

static void RenderGame(sf::RenderWindow* game_window, Minesweeper& game, AssetManager& assets, bool isInLeaderboard = false){
    game_window->clear(sf::Color::White);
    render_buttons(game_window, game, assets);
    render_cells(game_window, game, assets, isInLeaderboard);
    DrawCounter(game, game_window, game.getFlaggedRemaining());
    DrawTimer(game, game_window, game.getElapsedTime());
    game_window->display();
}

#endif //PROJECT3_GAMESCREEN_H