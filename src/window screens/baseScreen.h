#ifndef MINESWEEPER_BASESCREEN_H
#define MINESWEEPER_BASESCREEN_H
#include "../game fundamentals/gameData.h"
#include "../game fundamentals/text.h"

class baseScreen{
protected:
    // Game attributes
    gameData* gameDataPtr;

    // Window attributes
    sf::RenderWindow window;
    int windowWidth;
    int windowHeight;
    float windowWidthCenter;
    float windowHeightCenter;
    sf::Font windowFont;

    // Basic window operations
    baseScreen(gameData* gameDataPtr, int windowWidth, int windowHeight);
    void reloadWindow();
    virtual void run() = 0;
};

#endif //MINESWEEPER_BASESCREEN_H
