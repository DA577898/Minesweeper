#include <string>
#include "GameSettings.h"
#include "MinesweeperCore.h"
#include "GameScreen.h"
#include "WelcomeScreen.h"

using namespace std;

int main() {
    GameSettings current_settings("files/config.cfg");

    string username = WelcomeScreen(current_settings);
    // Check if user quit to close program
    if(!username.empty()) {
        Minesweeper game(current_settings, username);
        GameScreen(game);
    }
    return 0;
}