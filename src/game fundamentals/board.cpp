#include "board.h"

board::board() {
    // Do nothing. This is just here so a gameScreen object can create an empty board at the start
}

// Use to load in all the data from after a board has already been created
void board::loadData(sf::RenderWindow* windowPtr, gameData* gameDataPtr) {
    this->windowPtr = windowPtr;
    this->gameDataPtr = gameDataPtr;
    counter = gameDataPtr->getMineNum();
    createBoard();
}

void board::createBoard() {
    for (int i = 0; i < gameDataPtr->getRowNum(); i++){
        std::vector<tile> row;
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            tile gameTile(windowPtr, gameDataPtr, i, j);
            row.push_back(gameTile);
        }
        boardMatrix.push_back(row);
    }
}

void board::setUpBoardWithMines(int row, int col) {
    addMinesToBoard(row, col);
    for (int i = 0; i < gameDataPtr->getRowNum(); i++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            getAdjacentMines(boardMatrix[i][j]);
            boardMatrix[i][j].calcNumberAdjacentMines();
        }
    }
}

void board::addMinesToBoard(int row, int col) {
    int randomRow, randomCol;
    std::string randomPair;
    std::vector<std::string> completedRandomPairs;
    completedRandomPairs.push_back("(" + std::to_string(row) + ", " + std::to_string(col) + ")");
    bool addedMine;

    for (int i = 0 ; i < gameDataPtr->getMineNum(); i++){
        addedMine = false;
        while (!addedMine){
            randomRow = std::rand() % gameDataPtr->getRowNum();
            randomCol = std::rand() % gameDataPtr->getColNum();
            randomPair = "(" + std::to_string(randomRow) + ", " + std::to_string(randomCol) + ")";

            if (!checkIfInVector(completedRandomPairs, randomPair)){
                boardMatrix[randomRow][randomCol].setHasMine(true);
                completedRandomPairs.push_back(randomPair);
                addedMine = true;
            }
        }
    }
}

bool board::checkIfInVector(std::vector<std::string> vec, std::string value) {
    for (int i = 0; i < vec.size(); i++){
        if (vec[i] == value){
            return true;
        }
    }
    return false;
}

void board::getAdjacentMines(tile& gameTile) {
    int directions[8][2] = {
            {-1, -1}, // Up one row, left one col
            {-1, 0}, // Up one row, same col
            {-1, 1}, // Up one row, right one col
            {0, -1}, // Same row, left one col
            {0, 1}, // Same row, right one col
            {1, -1}, // Down one row, left one col
            {1, 0}, // Down one row, same col
            {1, 1}, // Down one row, right one col
    };

    for (auto direction : directions){
        int newRow = gameTile.getRow() + direction[0];
        int newCol = gameTile.getCol() + direction[1];

        if (checkIfValidPosition(newRow, newCol)){
            gameTile.getAdjacentTiles().push_back(&boardMatrix[newRow][newCol]);
        }
        else{
            gameTile.getAdjacentTiles().push_back(nullptr);
        }
    }
}

bool board::checkIfValidPosition(int newRow, int newCol) {
    return !(newRow >= gameDataPtr->getRowNum() || newRow < 0 || newCol >= gameDataPtr->getColNum() || newCol < 0);
}

void board::displayBoard() {
    for (int i = 0; i < gameDataPtr->getRowNum(); i ++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            boardMatrix[i][j].displayTile();
        }
    }
}

void board::displayBoardWhileGamePaused() {
    for (int i = 0; i < gameDataPtr->getRowNum(); i ++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            boardMatrix[i][j].displayTileWhileGamePaused();
        }
    }
}

void board::incrementCounter(bool state) {
    if (state){
        counter -= 1;
    }
    else{
        counter += 1;
    }
}

bool board::revealGivenTile(tile* tile) {
    if (tile->getHasMine() && !tile->getHasFlag()) {
        return true;
    }
    revealNearbyTiles(tile);
    return false;
}

void board::revealNearbyTiles(tile* tile) {
    // Base case: tile has a mine, has already been revealed, or has a flag
    if (tile->getHasMine() || !tile->getIsHidden() || tile->getHasFlag()){
        return;
    }
        // Base case: tile "has" a number (meaning it is adjacent to a mine)
    else if (tile->getHasNumber()){
        if (tile->getHasFlag()){
            tile->setHasFlag(false);
            counter += 1;
        }
        tile->revealTile();
        return;
    }
        // Recursive case: tile is not a mine, has not yet been revealed, and does not have a flag or number
    else{
        // Reveal tile
        if (tile->getHasFlag()){
            tile->setHasFlag(false);
            counter += 1;
        }
        tile->revealTile();

        // Recursive call
        for (int i = 0; i < tile->getAdjacentTiles().size(); i ++){
            if (tile->getAdjacentTiles()[i] != nullptr){
                revealNearbyTiles(tile->getAdjacentTiles()[i]);
            }
        }
    }
}

void board::revealAllMines(bool status, bool gameLost) {
    for (int i = 0; i < gameDataPtr->getRowNum(); i++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            if (boardMatrix[i][j].getHasMine()){
                if (status){
                    boardMatrix[i][j].revealMine(gameLost);
                }
                else{
                    boardMatrix[i][j].hideMine();
                }
            }
        }
    }
}

bool board::checkIfAllSafeTilesRevealed() {
    for (int i = 0; i < gameDataPtr->getRowNum(); i++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            if (!boardMatrix[i][j].getHasMine()){
                if (boardMatrix[i][j].getIsHidden()){
                    return false;
                }
            }
        }
    }
    return true;
}

void board::placeAllFlags() {
    for (int i = 0; i < gameDataPtr->getRowNum(); i++){
        for (int j = 0; j < gameDataPtr->getColNum(); j++){
            if (boardMatrix[i][j].getHasMine()){
                boardMatrix[i][j].setHasFlag(true);
            }
        }
    }
    counter = 0;
}

std::vector<std::vector<tile>>* board::getBoardMatrix() {
    return &boardMatrix;
}

int board::getCounter() {
    return counter;
}
