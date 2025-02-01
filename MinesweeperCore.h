//
// Created by Daniel on 4/16/2024.
//
#include "GameSettings.h"
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iostream>
#ifndef PROJECT3_MINESWEEPEERCORE_H
#define PROJECT3_MINESWEEPEERCORE_H

class Minesweeper{
    int num_columns, num_rows, num_mines, flagged_remaining, passed_seconds;
    string username, game_status;
    vector<vector<bool>> mine_tiles, discovered_tiles, flagged_tiles;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now(), time_pause_started;
    bool timer_paused = false, is_paused = false, is_debug_mode = false, is_first_play = true;
    GameSettings settings;

    void ClearBoard() {
        mine_tiles.clear();
        discovered_tiles.clear();
        flagged_tiles.clear();
        flagged_remaining = num_mines;

        for (int i = 0; i < num_rows; i++) {
            vector<bool> current_mines, current_discovered, current_flagged;
            for (int j = 0; j < num_columns; j++) {
                current_mines.push_back(false);
                current_discovered.push_back(false);
                current_flagged.push_back(false);
            }
            mine_tiles.push_back(current_mines);
            discovered_tiles.push_back(current_discovered);
            flagged_tiles.push_back(current_flagged);
        }
    }

    void ClearTile(int x, int y){
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_columns; j++) {
                if (!mine_tiles[i][j]) { // if current cell is clear
                    mine_tiles[i][j] = true;
                    mine_tiles[x][y] = false;
                    return;
                }
            }
        }
    }

    static bool IsSurrounding(int centerx, int centery, int posx, int posy, int rad){ // check if the point at posx posy is surrounding the center within the given radius
        return abs(centerx - posx) <= rad & abs(centery - posy) <= rad;
    }

    void ClearOutSpace(int x, int y){ // clear surrounding space (for first play)
        vector<sf::Vector2<int>> surrounding_mines;
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                if ((i >= 0 & i < num_rows) & (j >= 0 & j < num_columns) & !(x==i & y==j)) {
                    if(mine_tiles[i][j]) {
                        surrounding_mines.push_back({i, j}); // get surrounding mines
                    }
                }
            }
        }

        int ClearCells = ((num_columns * num_rows) - num_mines);

        if(mine_tiles[x][y]) {
            ClearTile(x, y);
            ClearCells--;
        }
        int numItr = min(ClearCells, (int)surrounding_mines.size());
        for(int i = 0; i < numItr; i++){ // then, remove as many of the surrounding mines as possible. do so randomly to avoid the same pattern
            int randomIndex = rand() % (int)(surrounding_mines.size());

            sf::Vector2<int> current = surrounding_mines.at(randomIndex);
            for (int j = 0; j < num_rows; j++) {
                for (int k = 0; k < num_columns; k++) {
                    if (!mine_tiles[j][k] & !IsSurrounding(x, y, j, k, 1) & !flagged_tiles[current.x][current.y]) { // if current cell is clear, and not flagged (who flags before the first play anyways?)
                        mine_tiles[j][k] = true;
                        mine_tiles[current.x][current.y] = false;
                        j = num_rows, k = num_columns;
                    }
                }
            }
            surrounding_mines.erase(surrounding_mines.begin() + randomIndex);
        }
    }
public:
    Minesweeper(GameSettings& settings, string username){
        num_columns = settings.getColumns();
        num_rows = settings.getRows();
        num_mines = settings.getMines();
        flagged_remaining = num_mines;
        passed_seconds = 0;
        this->username = username;
        this->settings = settings;
        GenerateBoard();
    }

    void GenerateBoard() {
        game_status = "happy";
        ClearBoard();
        int mines_placed = 0;
        while (mines_placed < num_mines) {
            int random_row = rand() % num_rows;
            int random_column = rand() % num_columns;

            if (!mine_tiles.at(random_row).at(random_column)) {
                mine_tiles.at(random_row).at(random_column) = true;
                mines_placed++;
            }
        }
        passed_seconds = 0;
        start_time = std::chrono::high_resolution_clock::now();
        resumeTimer();
        is_first_play = true;
    }

    int getNumRows() const {return num_rows;}
    int getNumColumns() const {return num_columns;}
    bool getIsPaused() const {return is_paused;}
    bool getIsDebugMode() const{return is_debug_mode;}

    int getFlaggedRemaining() const {return flagged_remaining;}
    string getGameStatus() const {return game_status;}

    string getUsername() {return username;}

    bool getIsDiscovered(int x, int y) {return discovered_tiles[x][y];}
    bool getIsFlagged(int x, int y){return flagged_tiles[x][y];}
    bool getIsMine(int x, int y){return mine_tiles[x][y];}

    int getCell(int row, int column) {
        if (mine_tiles[row][column]) { return -1; }
        int surrounding_mines = 0;
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = column - 1; j <= column + 1; j++) {
                if (i < num_rows && j < num_columns && j >= 0 && i >= 0) {
                    if (mine_tiles[i][j]) { surrounding_mines++; }
                }
            }
        }
        return surrounding_mines;
    }

    int getFlags(int row, int column) {
        if (flagged_tiles[row][column]) { return -1; }
        int num_flags = 0;
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = column - 1; j <= column + 1; j++) {
                if (i < num_rows && j < num_columns && j >= 0 && i >= 0) {
                    if (flagged_tiles[i][j]) { num_flags++; }
                }
            }
        }
        return num_flags;
    }

    int getElapsedTime(){
        if (!timer_paused) return (chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() + passed_seconds) / 1000;
        else return passed_seconds / 1000;

    }

    void pauseTimer(){
        if(!timer_paused) passed_seconds += chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
        timer_paused = true;
    }

    void resumeTimer(){
        if(timer_paused) start_time = std::chrono::high_resolution_clock::now();
        timer_paused = false;
    }

    void pause(){
        if (!is_paused & !timer_paused){
            pauseTimer();
        } else if (is_paused & timer_paused){
            resumeTimer();
        }
        is_paused = !is_paused;
    }

    bool toggleDebugMode(){
        is_debug_mode = !is_debug_mode;
        return is_debug_mode;
    }

    bool flagTile(int x, int y) {
        if (game_status == "happy" & !is_paused & !discovered_tiles[x][y])
            flagged_tiles[x][y] = not flagged_tiles[x][y];
            if (flagged_tiles[x][y]) flagged_remaining--; else flagged_remaining++;
        }
        return flagged_tiles[x][y];
    }

    void discoverTile(int x, int y, bool called_from_chord = false) {
        if(flagged_tiles[x][y]){return;} // if flagged cell, do nothing.
        else if (is_first_play & (getCell(x, y) != 0)){ // guarantee that a game can be played if there's enough free spots, and if possible, it starts at a blank tile
            ClearOutSpace(x, y);
            is_first_play = false;
            discoverTile(x, y);
        } else if (getCell(x, y) == -1) { // if it lands on a mine, discover all tiles, player lost :(
            for (int i = 0; i < num_rows; i++) {
                for(int j = 0; j < num_columns; j++){
                    discovered_tiles[i][j] = discovered_tiles[i][j] || mine_tiles[i][j];
                }
            }
            game_status = "lose";
            pauseTimer();
        } else if ((getFlags(x, y) > 0 & getFlags(x, y) == getCell(x, y) & !called_from_chord) || ((getCell(x, y) == 0) & !discovered_tiles[x][y])) {
            is_first_play = false;
            discovered_tiles[x][y] = true;
            for (int i = x - 1; i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    if (i < num_rows && j < num_columns && j >= 0 && i >= 0 && !(i == x && j == y)) { // boundaries
                        discoverTile(i, j, true);
                    }
                }
            }
        } else if (getCell(x, y) > 0) {
            is_first_play = false;
            discovered_tiles[x][y] = true;
        } else if (discovered_tiles[x][y] & (getFlags(x, y) > 0) & (getFlags(x, y) == getCell(x, y))){
            is_first_play = false;
            for (int i = x - 1; i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    if (i < num_rows && j < num_columns && j >= 0 && i >= 0 && !(i == x && j == y) && flagged_tiles[i][j]) { // boundaries
                        discovered_tiles[i][j] = true;
                    }
                }
            }
        }
        is_first_play = false;
        checkWin();
    }
    bool checkWin(){
        if(!is_first_play) { // can't win if it's before your first play. edge case when all tiles are mines
            for (int i = 0; i < num_rows; i++) {
                for (int j = 0; j < num_columns; j++) {
                    if (mine_tiles[i][j] == discovered_tiles[i][j]) {
                        return false;
                    }
                }
            }
            flagged_tiles = mine_tiles;
            flagged_remaining = 0;
            game_status = "win";
            return true;
        }
        return false;
    }
    GameSettings getSettings(){
        return settings;
    }
};

#endif //PROJECT3_MINESWEEPEERCORE_H
