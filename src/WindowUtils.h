//
// Created by Daniel on 4/18/2024.
//

#ifndef PROJECT3_WINDOWUTILS_H
#define PROJECT3_WINDOWUTILS_H
#include <cmath>

#pragma once

class AssetManager{
    map<string, sf::Texture> assets;
public:
    AssetManager(bool load){
        vector<string> assetNames{"debug", "face_happy", "face_lose", "face_win", "flag", "leaderboard", "mine","number_1",
                                  "number_2","number_3","number_4","number_5","number_6","number_7","number_8", "pause",
                                  "play", "tile_hidden", "tile_revealed"};
        for(const auto& i : assetNames){
            sf::Texture current;
            current.loadFromFile("files/images/" + i + ".png");
            assets.emplace(i, current);
        }
    }

    sf::Texture& operator()(const string& assetName){
        return assets.at(assetName);
    }
};

static void DrawSprite(sf::RenderWindow* window, const string& texture, float x, float y, AssetManager& assets){

    sf::Sprite current;
    current.setTexture(assets(texture));
    current.setPosition(x, y);
    window->draw(current);
}

static void DrawText(sf::RenderWindow* window, string text, float x, float y, int font_size, bool is_bold, bool is_underlined, sf::Color color){
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text current(text, font, font_size);

    sf::FloatRect textRect = current.getLocalBounds();
    current.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    current.setPosition(sf::Vector2f(x, y));
    if(is_bold){current.setStyle(sf::Text::Bold);}
    if(is_underlined){current.setStyle(sf::Text::Underlined);}
    current.setFillColor(color);
    window->draw(current);
}

static string checkButtonCollision(Minesweeper& game, sf::Vector2<int> pos, sf::Event event){
    if(event.mouseButton.button == sf::Mouse::Left) {
        if ((pos.x > ((game.getNumColumns() * 32.0) / 2.0) - 32) &&
            (pos.y > (32 * (game.getNumRows() + 0.5))) &&
            (pos.x < 64 + ((game.getNumColumns() * 32.0) / 2.0) - 32) &&
            (pos.y < 64 + (32 * (game.getNumRows() + 0.5)))) {
            return "face";
        }
        if ((pos.x > ((game.getNumColumns() * 32.0)) - 304) && (pos.y > (32 * (game.getNumRows() + 0.5))) &&
            (pos.x < 64 + ((game.getNumColumns() * 32.0)) - 304) &&
            (pos.y < 64 + (32 * (game.getNumRows() + 0.5)))) {
            return "debug";
        }
        if ((pos.x > ((game.getNumColumns() * 32.0)) - 240) && (pos.y > (32 * (game.getNumRows() + 0.5))) &&
            (pos.x < 64 + ((game.getNumColumns() * 32.0)) - 240) &&
            (pos.y < 64 + (32 * (game.getNumRows() + 0.5)))) {
                return "pause/play";
        }
        if ((pos.x > ((game.getNumColumns() * 32.0)) - 176) && (pos.y > (32 * (game.getNumRows() + 0.5))) &&
            (pos.x < 64 + ((game.getNumColumns() * 32.0)) - 176) &&
            (pos.y < 64 + (32 * (game.getNumRows() + 0.5)))) {
            return "leaderboard";
        }
        if ((pos.x >= 0 && pos.x <= game.getNumColumns() * 32 && pos.y >= 0 &&
             pos.y <= game.getNumRows() * 32)) {
            string x = to_string(floor(pos.y / 32.0));
            string y = to_string(floor(pos.x / 32.0));
            return "tile_left," + x + "," + y;
        }
    } else if (event.mouseButton.button == sf::Mouse::Right){
        if ((pos.x >= 0 && pos.x <= game.getNumColumns() * 32 && pos.y >= 0 &&
             pos.y <= game.getNumRows() * 32)) {
            string x = to_string(floor(pos.y / 32));
            string y = to_string(floor(pos.x / 32));
            if(floor(pos.y / 32) == 16){return "none";}
            return "tile_right," + x + "," + y;
        }
    }
    return "none";
}
static void DrawDigit(sf::RenderWindow* window, int digit, int x, int y){
    sf::Texture current_texture;
    if(digit == '-'){
        current_texture.loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));
    } else if (digit < 10) {
        current_texture.loadFromFile("files/images/digits.png", sf::IntRect(21*digit, 0, 21, 32));
    } else {return;}

    sf::Sprite current_sprite;
    current_sprite.setTexture(current_texture);
    current_sprite.setPosition(x, y);
    window->draw(current_sprite);
}
static void DrawCounter(Minesweeper& game, sf::RenderWindow* window, int value){
    if(value < 0){
        DrawDigit(window, '-', 12, 32*(game.getNumRows()+0.5)+16);
        value*=-1;
    }
    DrawDigit(window, (int)(value%1000)/100, 33, 32*(game.getNumRows()+0.5)+16);
    DrawDigit(window, (int)(value%100)/10, 54, 32*(game.getNumRows()+0.5)+16);
    DrawDigit(window, (int)(value%10), 75, 32*(game.getNumRows()+0.5)+16);
}

static void DrawTimer(Minesweeper& game, sf::RenderWindow* window, int value){
    int minutes = value/60;
    int seconds = value%60;
    DrawDigit(window, (int)(minutes%100)/10, (game.getNumColumns()*32)-97, 32 * (game.getNumRows() + 0.5) + 16);
    DrawDigit(window, (int)(minutes%10), (game.getNumColumns()*32)-97+21, 32 * (game.getNumRows() + 0.5) + 16);
    DrawDigit(window, (int)(seconds%100)/10, (game.getNumColumns()*32)-54, 32 * (game.getNumRows() + 0.5) + 16);
    DrawDigit(window, (int)(seconds%10), (game.getNumColumns()*32)-54+21, 32 * (game.getNumRows() + 0.5) + 16);
}

#endif //PROJECT3_WINDOWUTILS_H
