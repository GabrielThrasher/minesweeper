#include "gameScreen.h"

gameScreen::gameScreen(gameData* gameDataPtr) : baseScreen(gameDataPtr, 32 * gameDataPtr->getColNum(),
                                                           32 * gameDataPtr->getRowNum() + 100){
    rowNum = gameDataPtr->getRowNum();
    colNum = gameDataPtr->getColNum();
    leaderboardAlreadyUpdated = false;
    minesPlaced = false;

    loadUIData();
    gameBoard.loadData(&window, gameDataPtr);
    controlPausedGame = false;
    leaderboardPausedGame = false;
    debugStatus = false;
    gameWon = false;
    gameLost = false;
    windowFont = gameDataPtr->getGameFont();
    if (colNum < 25){
        fontSize = 15;
    }
    else if (colNum < 30){
        fontSize = 18;
    }
    else{
        fontSize = 21;
    }
    hasDebugMessage = false;

    gameStartTime = std::chrono::high_resolution_clock::now();
    totalTimePaused = 0;
}

void gameScreen::loadUIData() {
    // Load UI buttons data
    UIButtons["face"] = gameDataPtr->getSpriteFormat("face_happy", 16 * colNum - 32,32 * rowNum + 16);
    UIButtons["debug"] = gameDataPtr->getSpriteFormat("debug", 32 * colNum - 304,32 * rowNum + 16);
    UIButtons["control"] = gameDataPtr->getSpriteFormat("pause",32 * colNum - 240,32 * rowNum + 16);
    UIButtons["leaderboard"] = gameDataPtr->getSpriteFormat("leaderboard", 32 * colNum - 176,32 * rowNum + 16);

    //  Load UI display data
    UIDisplays["counterNeg"] = gameDataPtr->getSpriteFormat("-char", 12, 32 * rowNum + 32);
    UIDisplays["counter1st"] = gameDataPtr->getSpriteFormat("digit_0", 33,32 * rowNum + 32);
    UIDisplays["counter2nd"] = gameDataPtr->getSpriteFormat("digit_0", 54,32 * rowNum + 32);
    UIDisplays["counter3rd"] = gameDataPtr->getSpriteFormat("digit_0", 75,32 * rowNum + 32);
    UIDisplays["timer1st"] = gameDataPtr->getSpriteFormat("digit_0", 32 * colNum - 97,32 * rowNum + 32);
    UIDisplays["timer2nd"] = gameDataPtr->getSpriteFormat("digit_0", 32 * colNum - 76,32 * rowNum + 32);
    UIDisplays["timer3rd"] = gameDataPtr->getSpriteFormat("digit_0", 32 * colNum - 54,32 * rowNum + 32);
    UIDisplays["timer4th"] = gameDataPtr->getSpriteFormat("digit_0", 32 * colNum - 33,32 * rowNum + 32);
}

void gameScreen::reloadWindow() {
    window.clear(sf::Color::White);
    if (!(controlPausedGame || leaderboardPausedGame)){
        gameBoard.displayBoard();
    }
    else{
        gameBoard.displayBoardWhileGamePaused();
    }
    displayUI();
    if (hasDebugMessage) window.draw(noMinesDebugMessage);
    if (gameWon || gameLost) window.draw(gameEndMessage);
    window.display();
}

void gameScreen::displayUI() {
    changeCounterDisplay();
    if (!(gameWon || gameLost)) runTimer();

    for (auto iter = UIButtons.begin(); iter != UIButtons.end(); iter++){
        window.draw(iter->second);
    }
    for (auto iter = UIDisplays.begin(); iter != UIDisplays.end(); iter++){
        if (iter->first == "counterNeg"){
            if (gameBoard.getCounter() < 0){
                window.draw(iter->second);
            }
        }
        else{
            window.draw(iter->second);
        }
    }
}

std::vector<int> gameScreen::getMouseClickedTilePos(int xCoor, int yCoor) {
    sf::FloatRect bounds;
    int xPos = 0, yPos = 0;
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            bounds = (*gameBoard.getBoardMatrix())[i][j].getBaseSprite().getGlobalBounds();
            if (bounds.contains(xCoor, yCoor)) {
                xPos = i, yPos = j;
            }
        }
    }
    return {xPos, yPos};
}

void gameScreen::changeCounterDisplay(){
    int counter = abs(gameBoard.getCounter());
    std::string prefix = "digit_", counterStr;

    if (counter < 10){
        counterStr = "00" + std::to_string(counter);
    }
    else if (counter < 100){
        counterStr = "0" + std::to_string(counter);
    }
    else{
        counterStr = std::to_string(counter);
    }

    UIDisplays["counter1st"] = gameDataPtr->getSpriteFormat(prefix + counterStr[0],
                                                            33,32 * rowNum + 32);
    UIDisplays["counter2nd"] = gameDataPtr->getSpriteFormat(prefix + counterStr[1],
                                                            54,32 * rowNum + 32);
    UIDisplays["counter3rd"] = gameDataPtr->getSpriteFormat(prefix + counterStr[2],
                                                            75,32 * rowNum + 32);
}

void gameScreen::calculateGameRunTime() {
    gameEndTime = std::chrono::high_resolution_clock::now();
    gameRunTime = gameEndTime - gameStartTime;
    totalGameTime = gameRunTime.count() - totalTimePaused;

    // Game run time is less than or equal to 59 minutes, 59 seconds
    if (totalGameTime <= 3599) {
        minutesPlayed = totalGameTime / 60;
        secondsPlayed = totalGameTime % 60;
        std::string minutesPlayedStr, secondsPlayedStr;

        if (minutesPlayed < 10) {
            minutesPlayedStr = "0" + std::to_string(minutesPlayed);
        } else {
            minutesPlayedStr = std::to_string(minutesPlayed);
        }
        if (secondsPlayed < 10) {
            secondsPlayedStr = "0" + std::to_string(secondsPlayed);
        } else {
            secondsPlayedStr = std::to_string(secondsPlayed);
        }

        totalTime = minutesPlayedStr + ":" + secondsPlayedStr;
    }
}

void gameScreen::changeTimerDisplay(std::string minutesPlayedStr, std::string secondsPlayedStr) {
    std::string prefix = "digit_";
    UIDisplays["timer1st"] = gameDataPtr->getSpriteFormat(prefix + minutesPlayedStr[0],
                                                          32 * colNum - 97,32 * rowNum + 32);
    UIDisplays["timer2nd"] = gameDataPtr->getSpriteFormat(prefix + minutesPlayedStr[1],
                                                          32 * colNum - 76,32 * rowNum + 32);
    UIDisplays["timer3rd"] = gameDataPtr->getSpriteFormat(prefix + secondsPlayedStr[0],
                                                          32 * colNum - 54,32 * rowNum + 32);
    UIDisplays["timer4th"] = gameDataPtr->getSpriteFormat(prefix + secondsPlayedStr[1],
                                                          32 * colNum - 33,32 * rowNum + 32);
}

void gameScreen::runTimer() {
    calculateGameRunTime();
    changeTimerDisplay(totalTime.substr(0, totalTime.find(":")),
                       totalTime.substr(totalTime.find(":") + 1));
}

void gameScreen::pauseGame() {
    pauseStartTime = std::chrono::high_resolution_clock::now();
    UIButtons["control"] = gameDataPtr->getSpriteFormat("play",32 * colNum - 240,32 * rowNum + 16);
    reloadWindow();
}

void gameScreen::unpauseGame() {
    pauseEndTime = std::chrono::high_resolution_clock::now();
    pauseRunTime = pauseEndTime - pauseStartTime;
    totalTimePaused += pauseRunTime.count();
    UIButtons["control"] = gameDataPtr->getSpriteFormat("pause",32 * colNum - 240,32 * rowNum + 16);
}

void gameScreen::pauseGameFromControl() {
    controlPausedGame = true;
    pauseGame();
}

void gameScreen::unpauseGameFromControl() {
    controlPausedGame = false;
    unpauseGame();
}

void gameScreen::doClickedOnUIButtonSteps(int xCoor, int yCoor) {
    sf::FloatRect facePos = UIButtons["face"].getGlobalBounds();
    sf::FloatRect debugPos = UIButtons["debug"].getGlobalBounds();
    sf::FloatRect controlPos = UIButtons["control"].getGlobalBounds();
    sf::FloatRect leaderboardPos = UIButtons["leaderboard"].getGlobalBounds();

    // Face button was clicked on
    if (facePos.contains(xCoor, yCoor)){
        doFaceButtonSteps();
    }
        // Debug button was clicked on
    else if (debugPos.contains(xCoor, yCoor)){
        doDebugButtonSteps();
    }
        // Control button was clicked on
    else if (controlPos.contains(xCoor, yCoor)){
        doControlButtonSteps();
    }
        // Leaderboard button was clicked on
    else if (leaderboardPos.contains(xCoor, yCoor)) {
        doLeaderboardButtonSteps();
    }
}

void gameScreen::doFaceButtonSteps() {
    window.close();

    // Start new game
    gameScreen gameScreen(gameDataPtr);
    gameScreen.run();
}

void gameScreen::doDebugButtonSteps() {
    if (!minesPlaced){
        noMinesDebugMessage = getText(windowFont,
                                      "No mines generated\nyet! Left-click a\ntile first.",
                                      fontSize,70 + 6.3f * colNum, 32 * rowNum + 48,
                                      "black", "bold");
        hasDebugMessage = true;
    }
    else if (!(gameWon || gameLost) && !(controlPausedGame || leaderboardPausedGame)){
        debugStatus = !debugStatus;
        gameBoard.revealAllMines(debugStatus, false);
    }
}

void gameScreen::doControlButtonSteps() {
    if (!(gameWon || gameLost)){
        if (controlPausedGame){
            unpauseGameFromControl();
        }
        else{
            pauseGameFromControl();
        }
    }
}

void gameScreen::doLeaderboardButtonSteps() {
    if (!gameWon) leaderboardPausedGame = true;
    if (!controlPausedGame){
        pauseGame();
    }
    if (gameWon) leaderboardPausedGame = true;

    if (!leaderboardAlreadyUpdated && gameWon){
        leaderboardAlreadyUpdated = true;
        leaderboardScreen leaderboardWindow(gameDataPtr, true);
        leaderboardWindow.run();
    }
    else{
        leaderboardScreen leaderboardWindow(gameDataPtr, false);
        leaderboardWindow.run();
    }

    // This automatically happens after the leaderboard window closes
    if (!controlPausedGame){
        unpauseGame();
    }
    leaderboardPausedGame = false;
    if (gameLost) reloadWindow();
}

void gameScreen::runRightClickedTileSteps(tile& tile, std::vector<int> posVec) {
    tile.setHasFlag(!tile.getHasFlag());
    gameBoard.incrementCounter(tile.getHasFlag());
    (*gameBoard.getBoardMatrix())[posVec[0]][posVec[1]] = tile;
}

void gameScreen::runLeftClickedTileSteps(tile& tile, std::vector<int> posVec) {
    int row = posVec[0], col = posVec[1];
    if (!minesPlaced){
        gameBoard.setUpBoardWithMines(row, col);
        tile = (*gameBoard.getBoardMatrix())[row][col];
        minesPlaced = true;
        hasDebugMessage = false;
    }

    bool clickedOnMine = gameBoard.revealGivenTile(&tile);
    (*gameBoard.getBoardMatrix())[row][col] = tile;
    if(clickedOnMine){
        runLostGameSteps();
    }
    else if (gameBoard.checkIfAllSafeTilesRevealed()){
        runWonGameSteps();
    }
}

void gameScreen::runLostGameSteps() {
    UIButtons["face"] = gameDataPtr->getSpriteFormat("face_lose",
                                                     16 * colNum - 32,32 * rowNum + 16);
    gameBoard.revealAllMines(true, true);
    gameLost = true;
    gameEndMessage = getText(windowFont,
                             "You clicked on a\nmine and lost!",
                             fontSize,70 + 6.3f * colNum, 32 * rowNum + 48,"black", "bold");
    reloadWindow();
}

void gameScreen::runWonGameSteps() {
    UIButtons["face"] = gameDataPtr->getSpriteFormat("face_win",
                                                     16 * colNum - 32,32 * rowNum + 16);
    gameBoard.revealAllMines(false, false);
    gameBoard.placeAllFlags();
    gameWon = true;
    gameEndMessage = getText(windowFont,
                             "Congrats, you avoided\nthe mines and won!",
                             fontSize,70 + 6.3f * colNum, 32 * rowNum + 48,"black", "bold");
    reloadWindow();
    gameDataPtr->getCurrentPlayer().totalTime = totalTime;
    gameDataPtr->getCurrentPlayer().minutes = minutesPlayed;
    gameDataPtr->getCurrentPlayer().seconds = secondsPlayed;
    doLeaderboardButtonSteps();
}

void gameScreen::run() {
    while(window.isOpen()) {
        sf::Event event;
        // Continuously refresh the visuals on the game window as long as the game hasn't ended and isn't paused
        if (!(gameWon || gameLost) && !controlPausedGame) reloadWindow();

        while(window.pollEvent(event)) {
            // Close program
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed){
                mousePos = sf::Mouse::getPosition(window);

                // If mouse was clicked above a certain y position, a tile was definitely clicked on
                if (mousePos.y <= rowNum * 32){
                    // Execute actions as long as the game hasn't ended and isn't paused
                    if (!(gameWon || gameLost) && !(controlPausedGame || leaderboardPausedGame)){
                        std::vector<int> posVec = getMouseClickedTilePos(mousePos.x, mousePos.y);
                        tile tile = (*gameBoard.getBoardMatrix())[posVec[0]][posVec[1]];

                        // Mouse right-clicked: Add/remove flag if tile is hidden; check if player won
                        if (event.mouseButton.button == sf::Mouse::Right && tile.getIsHidden()){
                            runRightClickedTileSteps(tile, posVec);
                        }
                            // Mouse left-clicked: Reveal given tile and non-mine nearby tiles if tile is hidden
                        else if (event.mouseButton.button == sf::Mouse::Left && tile.getIsHidden()){
                            runLeftClickedTileSteps(tile, posVec);
                        }
                    }
                }
                    // A button was potentially clicked; if so, do its steps
                else{
                    doClickedOnUIButtonSteps(mousePos.x, mousePos.y);
                }
            } // end mouse if statement
        } // end inner while loop
    } // end outer while loop
} // end run function
