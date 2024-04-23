#include "gameData.h"

void gameData::loadData() {
    // Get file streams
    std::ifstream boardData("files/config.cfg");
    std::ifstream leaderboardData("files/leaderboard.txt");

    // Get the data from the file streams
    getBoardInfo(boardData);
    getLeaderboardInfo(leaderboardData);
    boardData.close();
    leaderboardData.close();
    gameFont.loadFromFile("files/font.ttf");

    // Load in textures and sprites
    loadTextures(textureFiles, sizeof(textureFiles)/sizeof(textureFiles[0]),
                 "files/images/");
    loadSprites();
}

void gameData::getBoardInfo(std::ifstream& stream) {
    std::string temp;
    std::getline(stream, temp);
    colNum = stoi(temp);
    std::getline(stream, temp);
    rowNum = stoi(temp);
    std::getline(stream, temp);
    mineNum = stoi(temp);
}

void gameData::getLeaderboardInfo(std::ifstream& stream) {
    leaderboardPlayers.clear();
    std::string temp;
    while (std::getline(stream, temp)){
        player player;
        player.totalTime = temp.substr(0, temp.find(","));
        player.name = temp.substr(temp.find(",") + 2);
        player.minutes = stoi(player.totalTime.substr(0, player.totalTime.find(":")));
        player.seconds = stoi(player.totalTime.substr(player.totalTime.find(":") + 1));
        leaderboardPlayers.push_back(player);
    }
}

void gameData::loadTextures(std::string textureFiles[], int arraySize, std::string commonPrefix) {
    // Load in all given textures
    std::string keyName;
    std::string file;
    for (int i = 0; i < arraySize; i++){
        sf::Texture texture;
        file = textureFiles[i];
        keyName = file.substr(0, file.find(".png"));
        textures.emplace(keyName, texture);
        textures[keyName].loadFromFile(commonPrefix + file);
    }
}

void gameData::loadSprites() {
    // Create sprites from the saved textures
    for (auto iter = textures.begin(); iter != textures.end(); iter++){
        sf::Sprite sprite;

        // Special sprite creation for digits
        if (iter->first == "digits"){
            std::string keyName;
            for (int  i = 0; i <= 10; i++){
                sprite.setTexture(iter->second);
                sprite.setTextureRect(sf::IntRect(21*i, 0, 21, 32));
                if (i == 10){
                    keyName = "-char";
                }
                else{
                    keyName = "digit_" + std::to_string(i);
                }
                sprites[keyName] = sprite;
            }
        }

            // Standard sprite creation
        else{
            sprite.setTexture(iter->second);
            sprites[iter->first] = sprite;
        }
    }
}

void gameData::writeLeaderboardInfo(std::vector<player> updatedLeaderboardPlayers) {
    leaderboardPlayers.clear();
    for (int i = 0; i < updatedLeaderboardPlayers.size(); i++){
        leaderboardPlayers.push_back(updatedLeaderboardPlayers[i]);
    }

    std::ofstream leaderboardData("files/leaderboard.txt");
    leaderboardData.seekp(0);
    for (int i = 0; i < leaderboardPlayers.size(); i++){
        leaderboardData << leaderboardPlayers[i].totalTime + ", " + leaderboardPlayers[i].name << std::endl;
    }
    leaderboardData.close();
}

sf::Sprite gameData::getSpriteFormat(std::string name, int xCoor, int yCoor) {
    sf::Sprite sprite;
    sprite = sprites.at(name);
    sprite.setPosition(xCoor, yCoor);
    return sprite;
}

int gameData::getRowNum() {
    return rowNum;
}

int gameData::getColNum() {
    return colNum;
}

int gameData::getMineNum() {
    return mineNum;
}

std::vector<gameData::player>* gameData::getLeaderboardPlayers() {
    return &leaderboardPlayers;
}

sf::Font gameData::getGameFont() {
    return gameFont;
}

gameData::player& gameData::getCurrentPlayer() {
    return currentPlayer;
}

gameData::player gameData::getCopyOfCurrentPlayer() {
    return currentPlayer;
}

std::map<std::string, sf::Texture>* gameData::getTextures() {
    return &textures;
}

std::map<std::string, sf::Sprite>* gameData::getSprites() {
    return &sprites;
}

void gameData::displayGeneralGameData() {
    std::cout << "Board Info:" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Col Num: " << colNum << std::endl;
    std::cout << "Row Num: " << rowNum << std::endl;
    std::cout << "Mine Num: " << mineNum << std::endl;
    std::cout << "--------------------------" << std::endl << std::endl;

    std::cout << "Leaderboard Players:" << std::endl;
    for (int i = 0; i < leaderboardPlayers.size(); i++){
        std::cout << "--------------------------" << std::endl;
        std::cout << "Player Name: " << leaderboardPlayers[i].name << std::endl;
        std::cout << "Total Time: " << leaderboardPlayers[i].totalTime << std::endl;
        std::cout << "Minutes: " << leaderboardPlayers[i].minutes << std::endl;
        std::cout << "Seconds: " << leaderboardPlayers[i].seconds << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
}

void gameData::displayTextureAndSpriteData() {
    std::cout << "Sprite Names:" << std::endl;
    std::cout << "--------------------------" << std::endl;
    for (auto iter = sprites.begin(); iter != sprites.end(); iter++){
        std::cout << "-> " << iter->first << std::endl;
    }
    std::cout << "--------------------------" << std::endl << std::endl;

    std::cout << "Texture Names:" << std::endl;
    std::cout << "--------------------------" << std::endl;
    for (auto iter = textures.begin(); iter != textures.end(); iter++){
        std::cout << "-> " << iter->first << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
}

gameData::player& gameData::player::operator=(gameData::player otherPlayer) {
    this->name = otherPlayer.name;
    this->totalTime = otherPlayer.totalTime;
    this->minutes = otherPlayer.minutes;
    this->seconds = otherPlayer.seconds;
    return *this;
}

bool gameData::player::operator==(gameData::player otherPlayer) {
    return (this->minutes == otherPlayer.minutes && this->seconds == otherPlayer.seconds);
}

bool gameData::player::operator<(gameData::player otherPlayer) {
    if (this->minutes == otherPlayer.minutes){
        if (this->seconds < otherPlayer.seconds){
            return true;
        }
    }
    else if (this->minutes < otherPlayer.minutes){
        return true;
    }
    return false;
}
