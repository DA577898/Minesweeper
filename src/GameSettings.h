//
// Created by Daniel on 4/16/2024.
//
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

#ifndef PROJECT3_GAME_SETTINGS_H
#define PROJECT3_GAME_SETTINGS_H

using namespace std;

class GameSettings {
    int num_columns;
    int num_rows;
    int num_mines;
public:
    GameSettings(string path){
        ifstream config_file(path);
        string token;
        getline(config_file, token);
        num_columns = stoi(token);
        getline(config_file, token);
        num_rows = stoi(token);
        getline(config_file, token);
        num_mines = stoi(token);
    }
    GameSettings(){
        ifstream config_file("files/config.cfg");
        string token;
        getline(config_file, token);
        num_columns = stoi(token);
        getline(config_file, token);
        num_rows = stoi(token);
        getline(config_file, token);
        num_mines = stoi(token);
    }
    GameSettings(GameSettings& other){
        num_columns = other.num_columns;
        num_rows = other.num_rows;
        num_mines = other.num_mines;
    }

    int getRows() const{return num_rows;}
    int getColumns() const{return num_columns;}
    int getMines() const{return num_mines;}

    sf::Vector2<float> getWelcomeScreenDimensions(){
        sf::Vector2<float> coordinates((num_columns*32), (num_rows*32)+100);
        return coordinates;
    }
    sf::Vector2<float> getGameScreenDimensions(){
        sf::Vector2<float> coordinates((num_columns*32), ((num_rows*32) + 100));
        return coordinates;
    }
    sf::Vector2<float> getLeaderboardScreenDimensions(){
        sf::Vector2<float> coordinates((num_columns*16), ((num_rows*16) + 50));
        return coordinates;
    }
};
#endif //PROJECT3_GAME_SETTINGS_H
