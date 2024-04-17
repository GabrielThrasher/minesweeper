#include "welcomeScreen.h"

welcomeScreen::welcomeScreen(gameData *gameDataPtr) : baseScreen(gameDataPtr, 32 * gameDataPtr->getColNum(),
                                                                 32 * gameDataPtr->getRowNum() + 100){
    startTime = std::chrono::high_resolution_clock::now();
    cursorIndex = 0;
    enterKeyPressedPrematurely = false;
    wrongKeyPressed = false;
}

void welcomeScreen::displayWelcomeMenuText(){
    sf::Text menuTitleText = getText(windowFont, "WELCOME TO MINESWEEPER!", 24,
                                     windowWidthCenter, windowHeightCenter - 150,
                                     "white", "bold, underlined");
    sf::Text promptUsernameText = getText(windowFont, "Enter your name:", 20,
                                          windowWidthCenter, windowHeightCenter - 75,
                                          "white", "bold");
    sf::Text authorText = getText(windowFont, "By: Gabriel Thrasher", 18,
                                          windowWidthCenter, windowHeight - 20,
                                          "white", "none");
    window.draw(menuTitleText);
    window.draw(promptUsernameText);
    window.draw(authorText);
}

std::string welcomeScreen::getCursor() {
    endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = endTime - startTime;
    int multiple = 11; // multiple and modulo should be coprime
    int modulo = 13; // smaller modulo = faster cursor blink rate

    if ((int)floor(time.count() * multiple) % modulo == 0){
        return "";
    }
    else{
        return "|";
    }
}

void welcomeScreen::displayUserInput() {
    std::string userInputStr = unsignedCharVectorToString(userInput);
    std::string userInputStrFirstHalf = userInputStr.substr(0, cursorIndex);
    std::string userInputStrSecondHalf = userInputStr.substr(cursorIndex);
    std::string cursorStr = getCursor();
    float charSpacing = 7.0f;

    // Get the to-be-displayed text and adjust individual parts to always keep it centered as a whole
    sf::Text cursorText = getText(windowFont, cursorStr,
                                  18, windowWidthCenter + charSpacing *
                                  (cursorIndex - (int)userInputStrSecondHalf.length()),
                                  windowHeightCenter - 45,"yellow", "bold");
    sf::Text userInputTextFirstHalf = getText(windowFont, userInputStrFirstHalf,
                                              18, cursorText.getGlobalBounds().left - charSpacing *
                                              userInputStrFirstHalf.length(), windowHeightCenter - 45,
                                              "yellow", "bold");
    sf::Text userInputTextSecondHalf = getText(windowFont, userInputStrSecondHalf,
                                                18, cursorText.getGlobalBounds().left +
                                                cursorText.getGlobalBounds().width + charSpacing *
                                                userInputStrSecondHalf.length(),windowHeightCenter - 45,
                                                "yellow", "bold");

    window.draw(userInputTextFirstHalf);
    window.draw(cursorText);
    window.draw(userInputTextSecondHalf);
    displayHelperMessages(userInputStr);
}

void welcomeScreen::displayHelperMessages(std::string userInputStr) {
    if(userInputStr.length() == 10){
        sf::Text maxCharLimitText = getText(windowFont, "Username character\n  limit reached!",19,
                                            windowWidthCenter,windowHeightCenter,"black", "none");
        window.draw(maxCharLimitText);
    }
    else if (enterKeyPressedPrematurely){
        sf::Text noUsernameText = getText(windowFont, "To continue you must\ntype in an username!",18,
                                          windowWidthCenter,windowHeightCenter,"black", "none");
        window.draw(noUsernameText);
    }
    else if(wrongKeyPressed){
        sf::Text wrongKeyText = getText(windowFont, "    Digits and special\ncharacters aren't allowed!",
                                        18,windowWidthCenter,windowHeightCenter,"black", "none");
        window.draw(wrongKeyText);
    }
}

void welcomeScreen::reloadWindow() {
    window.clear(sf::Color::Blue);
    displayWelcomeMenuText();
    displayUserInput();
    window.display();
}

void welcomeScreen::doAlphaCharKeySteps(unsigned char alphaChar) {
    if (userInput.empty() || userInput.size() < 10){
        unsigned char updatedAlphaChar;

        if (userInput.empty()){
            updatedAlphaChar = (unsigned char) std::toupper(alphaChar);
        }
        else{
            updatedAlphaChar = (unsigned char) std::tolower(alphaChar);
        }

        userInput.insert(userInput.begin() + cursorIndex,updatedAlphaChar);
        cursorIndex++;
        enterKeyPressedPrematurely = false;
        wrongKeyPressed = false;
    }
}

void welcomeScreen::doEnterKeySteps() {
    if (!userInput.empty()){
        gameDataPtr->getCurrentPlayer().name = unsignedCharVectorToString(userInput);
        window.close();
        gameScreen gameScreen(gameDataPtr);
        gameScreen.run();
    }
    else{
        enterKeyPressedPrematurely = true;
        wrongKeyPressed = false;
    }
}

void welcomeScreen::doBackSpaceKeySteps() {
    if (!userInput.empty() && userInput.size() <= 10 && cursorIndex != 0) {
        userInput.erase(userInput.begin() + cursorIndex - 1);
        cursorIndex--;
    }
}

void welcomeScreen::doDeleteKeySteps() {
    if (!userInput.empty() && userInput.size() <= 10 && cursorIndex != userInput.size()) {
        userInput.erase(userInput.begin() + cursorIndex);
    }
}

void welcomeScreen::doRightArrowKeySteps() {
    if (cursorIndex != userInput.size()){
        cursorIndex++;
    }
}

void welcomeScreen::doLeftArrowKeySteps() {
    if (cursorIndex != 0){
        cursorIndex--;
    }
}

void welcomeScreen::doWrongKeySteps() {
    wrongKeyPressed = true;
    enterKeyPressedPrematurely = false;
}

void welcomeScreen::run() {
    while(window.isOpen()) {
        sf::Event event;
        // Continuously reload the screen if the player's username is not above 10 alpha chars
        if (userInput.size() <= 10) reloadWindow();

        while(window.pollEvent(event)) {
            // Close program
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Key was pressed
            else if (event.type == sf::Event::KeyPressed) {
                // Alpha key was pressed
                if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
                {
                    // Get the alpha character from the key code
                    doAlphaCharKeySteps(static_cast<unsigned char>(event.key.code - sf::Keyboard::A + 'a'));
                }
                // A non-alpha key was pressed
                else if (event.key.code == sf::Keyboard::Enter){
                    doEnterKeySteps();
                }
                else if (event.key.code == sf::Keyboard::BackSpace){
                    doBackSpaceKeySteps();
                }
                else if (event.key.code == sf::Keyboard::Delete){
                    doDeleteKeySteps();
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    doRightArrowKeySteps();
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    doLeftArrowKeySteps();
                }
                else{
                    doWrongKeySteps();
                }
            } // end event if statement
        } // end inner while loop
    } // end outer while loop
} // end run function
