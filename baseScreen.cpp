#include "baseScreen.h"

baseScreen::baseScreen(gameData* gameDataPtr, int windowWidth, int windowHeight){
    this->gameDataPtr = gameDataPtr;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    windowWidthCenter = windowWidth / 2;
    windowHeightCenter = windowHeight / 2;
    windowFont = gameDataPtr->getGameFont();
    window.create(sf::VideoMode(windowWidth ,windowHeight),
                  "Minesweeper",sf::Style::Close);
}
