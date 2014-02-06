#ifndef UTILITY_HPP
#define UTILITY_HPP
 
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

void centerOrigin(sf::Sprite &sprite);
void centerOrigin(sf::Text &text);

float degreeToRadian(float degree);
float radianToDegree(float radian);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

int randomInt(int range);

#endif