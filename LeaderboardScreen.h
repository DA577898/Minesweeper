//
// Created by Daniel on 4/18/2024.
//

#ifndef PROJECT3_LEADERBOARDSCREEN_H
#define PROJECT3_LEADERBOARDSCREEN_H
#pragma once

bool Leaderboard(Minesweeper& game);

static string getLeaderboardPlayers(){
    string players = "";
    ifstream leaderboard("files/leaderboard.txt");
    string line;
    int cur = 1;
    while(getline(leaderboard, line)){
        string time, name;

        istringstream stream(line);
        getline(stream, time, ',');
        getline(stream, name, ',');
        name = name.substr(1, name.length()); // removes space before name

        players += to_string(cur) + "\t" + time + "\t" + name + "\n\n";
        cur++;
    }
    return players;
}

static void addPlayerToLeaderboard(int time, string username){
    vector<pair<int, string>> players;
    ifstream leaderboard_file_in("files/leaderboard.txt");
    string line;
    while(getline(leaderboard_file_in, line)) {
        string token;
        pair<int, string> cur;
        istringstream stream(line);
        getline(stream, token, ',');
        cur.first = (int)(stoi(token.substr(0, 2)) * 60) + stoi(token.substr(3, 5));
        getline(stream, cur.second, ',');
        cur.second = cur.second.substr(1, cur.second.length());

        players.push_back(cur);
    }
    leaderboard_file_in.close();
    for(int i = 0; i < 5; i++){
        if(i >= players.size()){
            pair<int, string> current(time, username+"*"); // adds asterisk to new winner!
            players.push_back(current);
            break;
        }
        else if(players.at(i).first >= time){ // handle leaderboards with less than 5 players
            pair<int, string> current(time, username+"*"); // adds asterisk to new winner!
            if(players.size() < 5){
                players.push_back(players.at(players.size()-1)); // add last value to the end
                for (int j = players.size() - 1; j > i; j--) {
                    players.at(j) = players.at(j - 1);
                }
                players.at(i) = current;
                break;
            } else {
                for (int j = players.size() - 1; j > i; j--) {
                    players.at(j) = players.at(j - 1);
                }
                players.at(i) = current;
                break;
            }
        }
    }
    ofstream leaderboard_file_out("files/leaderboard.txt");
    bool first = true;
    for(auto i : players){
        int minutes = i.first/60;
        int seconds = i.first % 60;
        if(!first){leaderboard_file_out << "\n";} else {first = !first;} // edge case for newline
        leaderboard_file_out << to_string((int)(minutes%100)/10) << to_string((int)(minutes%10)) << ':'
                             << to_string((int)(seconds%100)/10) << to_string((int)(seconds%10)) << ", "
                             << i.second;
    }
    leaderboard_file_out.close();
}

static void removeAsterisks(){
    vector<pair<int, string>> players;
    ifstream leaderboard_file_in("files/leaderboard.txt");
    string line;
    while(getline(leaderboard_file_in, line)) {
        string token;
        pair<int, string> cur;
        istringstream stream(line);
        getline(stream, token, ',');
        cur.first = (int)(stoi(token.substr(0, 2)) * 60) + stoi(token.substr(3, 5));
        getline(stream, cur.second, ',');
        cur.second = cur.second.substr(1, cur.second.length());

        players.push_back(cur);
    }
    leaderboard_file_in.close();
    for(auto& player: players){
        if(player.second.substr(player.second.length()-1, player.second.length()) == "*"){
            player.second = player.second.substr(0, player.second.length()-1);
        }
    }
    ofstream leaderboard_file_out("files/leaderboard.txt");
    bool first = true;
    for(auto i : players){
        int minutes = i.first/60;
        int seconds = i.first % 60;
        if(!first){leaderboard_file_out << "\n";} else {first = !first;} // edge case for newline
        leaderboard_file_out << to_string((int)(minutes%100)/10) << to_string((int)(minutes%10)) << ':'
                             << to_string((int)(seconds%100)/10) << to_string((int)(seconds%10)) << ", "
                             << i.second;
    }
    leaderboard_file_out.close();
}

#endif //PROJECT3_LEADERBOARDSCREEN_H
