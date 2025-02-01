//
// Created by Daniel on 4/18/2024.
//

#ifndef PROJECT3_WELCOMESCREEN_H
#define PROJECT3_WELCOMESCREEN_H
#pragma once

string WelcomeScreen(GameSettings settings);

static char getLetter(int number){
    string vocab = "abcdefghijklmnopqrstuvwxyz";
    if(number >= 0 && number <= 25){
        return vocab[number];
    }
    return ' ';
}

#endif //PROJECT3_WELCOMESCREEN_H
