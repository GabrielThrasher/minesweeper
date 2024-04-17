#ifndef MINESWEEPER_LEADERBOARDSCREEN_H
#define MINESWEEPER_LEADERBOARDSCREEN_H
#include "baseScreen.h"

class leaderboardScreen : protected baseScreen, protected gameData{
private:
    std::vector<gameData::player>* leaderboardPlayersPtr;
    gameData::player currentPlayer;
    gameData::player lastPlaceLeaderboardPlayer;

public:
    leaderboardScreen(gameData* gameDataPtr, bool updateLeaderboard);
    void getLeaderboard(bool updateLeaderboard);
    void displayLeaderboardStats(int newTopPlayerIndex, std::vector<gameData::player>);
    void reloadWindow(bool updateLeaderboard);
    void run();
};

#endif //MINESWEEPER_LEADERBOARDSCREEN_H
