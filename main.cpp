#include "welcomeScreen.h"

int main() {
    // Load in game data (board & leaderboard info, fonts, textures, and sprites) from files
    gameData gameData;
    gameData.loadData();

    // Run the Welcome Screen (indirectly starting the entire program)
    welcomeScreen welcomeMenu(&gameData);
    welcomeMenu.run();

    return 0;
}
