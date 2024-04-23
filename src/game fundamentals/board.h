#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H
#include "tile.h"

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
    void setUpBoardWithMines(int row, int col);
    void addMinesToBoard(int row, int col);
    bool checkIfInVector(std::vector<std::string> vec, std::string value);
    void getAdjacentMines(tile& gameTile);
    bool checkIfValidPosition(int newRow, int newCol);

    // Mid-game functions
    void displayBoard();
    void displayBoardWhileGamePaused();
    void incrementCounter(bool state);
    bool revealGivenTile(tile* tile);
    void revealNearbyTiles(tile* tile);
    void revealAllMines(bool status, bool gameLost);
    bool checkIfAllSafeTilesRevealed();
    void placeAllFlags();

    // Getter functions
    std::vector<std::vector<tile>>* getBoardMatrix();
    int getCounter();
};

#endif //MINESWEEPER_BOARD_H
