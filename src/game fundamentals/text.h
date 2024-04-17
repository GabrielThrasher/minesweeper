#ifndef MINESWEEPER_TEXT_H
#define MINESWEEPER_TEXT_H
#include <SFML/Graphics.hpp>
#include <iostream>

// Common operations that can be done on the text objects used throughout the program
void setTextBasics(sf::Text &text, sf::Font& font, std::string message);
void setTextExtras(sf::Text &text, short int size, std::string color, std::string effects);
void setTextPosition(sf::Text &text, float x, float y);
sf::Text getText(sf::Font& font, std::string message, short int size, float x, float y,
                 std::string color="white", std::string effects="none");
std::string unsignedCharVectorToString(std::vector<unsigned char>& vec);

#endif //MINESWEEPER_TEXT_H
