#include "text.h"

void setTextBasics(sf::Text &text, sf::Font& font, std::string message) {
    text.setFont(font);
    text.setString(message);
}

void setTextExtras(sf::Text &text, short int size, std::string color, std::string effects) {
    text.setCharacterSize(size);

    if (color == "white"){
        text.setFillColor(sf::Color::White);
    }
    else if (color == "yellow"){
        text.setFillColor(sf::Color::Yellow);
    }
    else if (color == "black"){
        text.setFillColor(sf::Color::Black);
    }

    if (effects.find("bold") != std::string::npos && effects.find("underlined") != std::string::npos){
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
    else if (effects.find("bold") != std::string::npos){
        text.setStyle(sf::Text::Bold);
    }
    else if (effects.find("underlined") != std::string::npos){
        text.setStyle(sf::Text::Underlined);
    }
}

void setTextPosition(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

sf::Text getText(sf::Font& font, std::string message, short int size, float x, float y,
                 std::string color, std::string effects) {
    sf::Text text;
    setTextBasics(text, font, message);
    setTextExtras(text, size, color, effects);
    setTextPosition(text, x, y);
    return text;
}

std::string unsignedCharVectorToString(std::vector<unsigned char>& vec){
    std::string str(vec.begin(), vec.end());
    return str;
}
