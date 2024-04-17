#ifndef MINESWEEPER_WELCOMESCREEN_H
#define MINESWEEPER_WELCOMESCREEN_H
#include "gameScreen.h"
#include <cmath>

class welcomeScreen : protected baseScreen{
private:
    // Window attributes
    std::vector<unsigned char> userInput;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    int cursorIndex;
    bool enterKeyPressedPrematurely;
    bool wrongKeyPressed;

public:
    welcomeScreen(gameData *gameDataPtr);
    void displayWelcomeMenuText();
    std::string getCursor();
    void displayUserInput();
    void displayHelperMessages(std::string userInputStr);
    void reloadWindow();
    void getUsernameFormat();
    void doAlphaCharKeySteps(unsigned char alphaChar);
    void doEnterKeySteps();
    void doBackSpaceKeySteps();
    void doDeleteKeySteps();
    void doRightArrowKeySteps();
    void doLeftArrowKeySteps();
    void doWrongKeySteps();
    void run();
};

#endif //MINESWEEPER_WELCOMESCREEN_H
