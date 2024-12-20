#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H
#include "gameData.h"

class tile{
private:
    // Game attributes
    gameData* gameDataPtr;
    sf::RenderWindow* windowPtr;

    // Tile attributes
    sf::Sprite baseSprite;
    sf::Sprite topSprite;
    sf::Sprite prevTopSprite;
    sf::Sprite mineSprite;
    sf::Sprite emptySprite;
    sf::Sprite gamePauseBaseSprite;
    int row;
    int col;
    int xPos;
    int yPos;
    bool hasMine;
    bool hasFlag;
    bool isHidden;
    bool hasNumber;
    std::vector<tile*> adjacentTiles;
    int numberAdjacentMines;

public:
    tile(sf::RenderWindow* windowPtr, gameData* gameDataPtr, int row, int col);
    void displayTile();
    void displayTileWhileGamePaused();
    void calcNumberAdjacentMines();
    void revealTile();
    void revealMine(bool gameLost);
    void hideMine();

    // Getter functions
    sf::Sprite& getBaseSprite();
    sf::Sprite& getTopSprite();
    std::vector<tile*>& getAdjacentTiles();
    bool getHasMine();
    bool getHasFlag();
    bool getHasNumber();
    int getRow();
    int getCol();
    int getXPos();
    int getYPos();
    bool getIsHidden();
    int getNumAdjacentMines();

    // Setter functions
    void setBaseSprite(std::string spriteName);
    void setTopSprite(std::string spriteName);
    void setHasMine(bool state);
    void setHasFlag(bool state);
    void setIsHidden(bool state);
};

#endif //MINESWEEPER_TILE_H
