#include "tile.h"

tile::tile(sf::RenderWindow* windowPtr, gameData* gameDataPtr, int row, int col) {
    this->windowPtr = windowPtr;
    this->gameDataPtr = gameDataPtr;
    this->row = row;
    this->col = col;
    xPos = col * 32; // 32 as that's the width of the tile sprite
    yPos = row * 32; // 32 as that's the height of the tile sprite

    baseSprite = gameDataPtr->getSprites()->at("tile_hidden");
    gamePauseBaseSprite = gameDataPtr->getSprites()->at("tile_revealed");
    hasMine = false;
    hasFlag = false;
    hasNumber = false;
    isHidden = true;
    numberAdjacentMines = 0;
}

void tile::displayTile() {
    baseSprite.setPosition(xPos, yPos);
    windowPtr->draw(baseSprite);
    topSprite.setPosition(xPos, yPos);
    windowPtr->draw(topSprite);
    mineSprite.setPosition(xPos, yPos);
    windowPtr->draw(mineSprite);
}

void tile::displayTileWhileGamePaused() {
    gamePauseBaseSprite.setPosition(xPos, yPos);
    windowPtr->draw(gamePauseBaseSprite);
}

void tile::calcNumberAdjacentMines() {
    for (tile* tile : adjacentTiles){
        if (tile != nullptr){
            if (tile->getHasMine()){
                numberAdjacentMines++;
            }
        }
    }
    if (numberAdjacentMines > 0 && !hasMine){
        hasNumber = true;
    }
    else{
        hasNumber = false;
    };
}

void tile::revealTile() {
    isHidden = false;
    baseSprite = gameDataPtr->getSprites()->at("tile_revealed");
    if (hasMine){
        mineSprite = gameDataPtr->getSprites()->at("mine");
    }
    else if (hasNumber){
        topSprite = gameDataPtr->getSprites()->at("number_" + std::to_string(numberAdjacentMines));
    }
}

void tile::revealMine(bool gameLost){
    sf::Sprite temp;
    temp = topSprite;
    if (gameLost) baseSprite = gameDataPtr->getSprites()->at("tile_revealed");
    topSprite = prevTopSprite;
    prevTopSprite = temp;
    mineSprite = gameDataPtr->getSprites()->at("mine");
}

void tile::hideMine() {
    topSprite = prevTopSprite;
    mineSprite = emptySprite;
}

sf::Sprite& tile::getBaseSprite() {
    return baseSprite;
}

sf::Sprite& tile::getTopSprite() {
    return topSprite;
}

std::vector<tile*>& tile::getAdjacentTiles() {
    return adjacentTiles;
}

bool tile::getHasMine() {
    return hasMine;
}

bool tile::getHasFlag() {
    return hasFlag;
}

bool tile::getHasNumber() {
    return hasNumber;
}

int tile::getRow() {
    return row;
}

int tile::getCol() {
    return col;
}

int tile::getXPos() {
    return xPos;
}

int tile::getYPos() {
    return yPos;
}

bool tile::getIsHidden(){
    return isHidden;
}

int tile::getNumAdjacentMines() {
    return numberAdjacentMines;
}

void tile::setBaseSprite(std::string spriteName) {
    baseSprite = gameDataPtr->getSprites()->at(spriteName);
}

void tile::setTopSprite(std::string spriteName) {
    topSprite = gameDataPtr->getSprites()->at(spriteName);
}

void tile::setHasMine(bool state) {
    hasMine = state;
}

void tile::setHasFlag(bool state) {
    hasFlag = state;
    if (state){
        topSprite = gameDataPtr->getSprites()->at("flag");
    }
    else{
        topSprite = emptySprite;
    }
    prevTopSprite = topSprite;
}

void tile::setIsHidden(bool state) {
    isHidden = state;
}
