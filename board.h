#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H
#include "tile.h"
//#include <cstdlib>

class board{
private:
    // Game attributes
    gameData* gameDataPtr;
    sf::RenderWindow* windowPtr;
    std::vector<std::vector<tile>> boardMatrix;
    int counter;

public:
    // Board set up functions
    board();
    void loadData(sf::RenderWindow* windowPtr, gameData* gameDataPtr);
    void createBoard();
    void setUpBoardWithMines(int xPos, int yPos);
    void addMinesToBoard(int xPos, int yPos);
    bool checkIfInVector(std::vector<std::string> vec, std::string value);
    void getAdjacentMines(tile& gameTile, int& xPos, int& yPos);

    // Mid-game functions
    void displayBoard();
    void displayBoardWhileGamePaused();
    void incrementCounter(bool state);
    bool revealGivenTile(tile* tile);
    void revealNearbyTiles(tile* tile);
    void revealAllMines(bool status, bool gameLost);
    bool checkIfAllFlagsPlacedProperly();
    bool checkIfAllSafeTilesRevealed();
    void placeAllFlags();

    // Getter functions
    std::vector<std::vector<tile>>* getBoardMatrix();
    int getCounter();
};

#endif //MINESWEEPER_BOARD_H
