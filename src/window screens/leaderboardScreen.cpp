#include "leaderboardScreen.h"

leaderboardScreen::leaderboardScreen(gameData* gameDataPtr, bool updateLeaderboard) : baseScreen(gameDataPtr,
                                                                                                 16 * gameDataPtr->getColNum(),16 * gameDataPtr->getRowNum() + 50) {
    reloadWindow(updateLeaderboard);
}

void leaderboardScreen::getLeaderboard(bool updateLeaderboard) {
    leaderboardPlayersPtr = gameDataPtr->getLeaderboardPlayers();
    int newTopPlayerIndex = leaderboardPlayersPtr->size();

    if (updateLeaderboard){
        player currPlayer = gameDataPtr->getCopyOfCurrentPlayer();
        std::vector<gameData::player> updatedLeaderboardPlayers;

        // If the current player at least beat the last-placing leaderboard player's time
        if (currPlayer < (*leaderboardPlayersPtr)[leaderboardPlayersPtr->size() - 1]){
            int idx = 0;
            while (idx < leaderboardPlayersPtr->size()){
                if (currPlayer < (*leaderboardPlayersPtr)[idx]){
                    updatedLeaderboardPlayers.push_back(currPlayer);
                    newTopPlayerIndex = idx;
                    break;
                }
                else if (currPlayer == (*leaderboardPlayersPtr)[idx] && idx != leaderboardPlayersPtr->size() - 1){
                    // Accounts for if numerous people on the leaderboard have the same total time
                    int lastEqualPlayerIndex;
                    for (int i = idx; i < leaderboardPlayersPtr->size(); i++){
                        if (!(currPlayer == (*leaderboardPlayersPtr)[idx]) && idx != leaderboardPlayersPtr->size() - 1){
                            lastEqualPlayerIndex = i - 1;
                        }
                        else{
                            lastEqualPlayerIndex = i;
                        }
                    }
                    if (lastEqualPlayerIndex != leaderboardPlayersPtr->size() - 1){
                        updatedLeaderboardPlayers.push_back((*leaderboardPlayersPtr)[idx]);
                        updatedLeaderboardPlayers.push_back(currPlayer);
                        newTopPlayerIndex = idx + 1;
                        break;
                    }
                    else{
                        updatedLeaderboardPlayers.push_back((*leaderboardPlayersPtr)[idx]);
                    }
                }
                else{
                    updatedLeaderboardPlayers.push_back((*leaderboardPlayersPtr)[idx]);
                }
                idx++;
            }

            // Add any remaining players that are ranked after the new addition (current player) but still in the top 5
            for (int i = newTopPlayerIndex; i < leaderboardPlayersPtr->size() - 1; i++){
                updatedLeaderboardPlayers.push_back((*leaderboardPlayersPtr)[i]);
            }
            *leaderboardPlayersPtr = updatedLeaderboardPlayers;
            writeLeaderboardInfo(updatedLeaderboardPlayers);
        }
    }
    displayLeaderboardStats(newTopPlayerIndex, *leaderboardPlayersPtr);
}

void leaderboardScreen::displayLeaderboardStats(int newTopPlayerIndex,
                                                std::vector<gameData::player> updatedLeaderboardPlayers) {
    sf::Text leaderboardTitle = getText(windowFont, "LEADERBOARD", 20, windowWidthCenter,
                                        windowHeightCenter - 120, "white", "bold, underlined");
    window.draw(leaderboardTitle);

    std::string leaderboardStatsStr;
    for (int i = 0; i < updatedLeaderboardPlayers.size(); i++){
        leaderboardStatsStr += std::to_string(i + 1) + ".\t" + updatedLeaderboardPlayers[i].totalTime + "\t";
        if (i == newTopPlayerIndex){
            leaderboardStatsStr += updatedLeaderboardPlayers[i].name + "*";
        }
        else{
            leaderboardStatsStr += updatedLeaderboardPlayers[i].name;
        }
        if (i != updatedLeaderboardPlayers.size() - 1){
            leaderboardStatsStr += "\n\n";
        }
    }
    sf::Text leaderBoardStats = getText(windowFont, leaderboardStatsStr, 18, windowWidthCenter,
                                        windowHeightCenter + 20, "white", "bold");
    window.draw(leaderBoardStats);
}

void leaderboardScreen::reloadWindow(bool updateLeaderboard) {
    window.clear(sf::Color::Blue);
    getLeaderboard(updateLeaderboard);
    window.display();
}

void leaderboardScreen::run() {
    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            // Close program
            if (event.type == sf::Event::Closed) window.close();

        } // end inner while loop
    } // end outer while loop
} // end run function
