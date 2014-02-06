#include "ResourceHolder.cpp"

#include "ResourceIdentifier.hpp"

#include <SFML/Graphics.hpp>

template class ResourceHolder<Textures::ID, sf::Texture>;
template class ResourceHolder<Fonts::ID, sf::Font>;
