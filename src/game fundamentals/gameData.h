#ifndef MINESWEEPER_GAMEDATA_H
#define MINESWEEPER_GAMEDATA_H
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>

class gameData{
protected:
    // Player model
    struct player{
        std::string name;
        std::string totalTime;
        int minutes;
        int seconds;

        player& operator=(player otherPlayer);
        bool operator==(player otherPlayer);
        bool operator<(player otherPlayer);
    };

private:
    // Board info
    int rowNum;
    int colNum;
    int mineNum;

    // General game info
    std::vector<player> leaderboardPlayers;
    sf::Font gameFont;
    player currentPlayer;

    // Sprite & texture data
    std::string textureFiles[20] = {"debug.png","face_happy.png", "face_lose.png", "face_win.png",
                                "flag.png", "leaderboard.png", "mine.png", "number_1.png",
                                "number_2.png", "number_3.png", "number_4.png", "number_5.png",
                                "number_6.png", "number_7.png", "number_8.png", "pause.png",
                                "play.png", "tile_hidden.png", "tile_revealed.png", "digits.png"
                                };
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;

public:
    // Program startup functions
    void loadData();
    void getBoardInfo(std::ifstream& stream);
    void getLeaderboardInfo(std::ifstream& stream);
    void loadTextures(std::string textureFiles[], int arraySize, std::string commonPrefix="");
    void loadSprites();

    // Update leaderboard function
    void writeLeaderboardInfo(std::vector<player> updatedLeaderboardPlayers);

    // Getter functions
    sf::Sprite getSpriteFormat(std::string name, int xCoor, int yCoor);
    int getRowNum();
    int getColNum();
    int getMineNum();
    std::vector<player>* getLeaderboardPlayers();
    sf::Font getGameFont();
    player& getCurrentPlayer();
    player getCopyOfCurrentPlayer();
    std::map<std::string, sf::Texture>* getTextures();
    std::map<std::string, sf::Sprite>* getSprites();

    // Debugging functions
    void displayGeneralGameData();
    void displayTextureAndSpriteData();
};

#endif //MINESWEEPER_GAMEDATA_H
