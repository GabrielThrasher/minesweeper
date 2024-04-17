#ifndef MINESWEEPER_GAMESCREEN_H
#define MINESWEEPER_GAMESCREEN_H
#include "board.h"
#include "leaderboardScreen.h"
#include <chrono>

class gameScreen : protected baseScreen{
private:
    // Game attributes
    short int rowNum;
    short int colNum;
    bool leaderboardAlreadyUpdated;
    bool minesPlaced;
    bool gameWon;
    bool gameLost;

    // Time attributes
    std::chrono::high_resolution_clock::time_point gameStartTime;
    std::chrono::high_resolution_clock::time_point gameEndTime;
    std::chrono::duration<double> gameRunTime;
    int totalGameTime;
    int minutesPlayed;
    int secondsPlayed;
    std::string totalTime;
    std::chrono::high_resolution_clock::time_point pauseStartTime;
    std::chrono::high_resolution_clock::time_point pauseEndTime;
    std::chrono::duration<double> pauseRunTime;
    int totalTimePaused;

    // Window attributes
    board gameBoard;
    bool controlPausedGame;
    bool leaderboardPausedGame;
    bool debugStatus;
    sf::Font windowFont;
    int fontSize;
    bool hasDebugMessage;

    // UI attributes
    std::map<std::string, sf::Sprite> UIButtons;
    std::map<std::string, sf::Sprite> UIDisplays;
    sf::Text gameEndMessage;
    sf::Text noMinesDebugMessage;

    // Mouse attributes
    sf::Vector2i mousePos;

public:
    gameScreen(gameData* gameDataPtr);
    void loadUIData();
    void reloadWindow();
    void displayUI();
    std::vector<int> getMouseClickedTilePos(int xCoor, int yCoor);
    void changeCounterDisplay();
    void calculateGameRunTime();
    void changeTimerDisplay(std::string minutesPlayedStr, std::string secondsPlayedStr);
    void runTimer();
    void pauseGame();
    void unpauseGame();
    void pauseGameFromControl();
    void unpauseGameFromControl();
    void doClickedOnUIButtonSteps(int xCoor, int yCoor);
    void doFaceButtonSteps();
    void doDebugButtonSteps();
    void doControlButtonSteps();
    void doLeaderboardButtonSteps();
    void runRightClickedTileSteps(tile& tile, std::vector<int> posVec);
    void runLeftClickedTileSteps(tile& tile, std::vector<int> posVec);
    void runLostGameSteps();
    void runWonGameSteps();
    void run();
};

#endif //MINESWEEPER_GAMESCREEN_H
