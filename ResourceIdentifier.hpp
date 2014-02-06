#ifndef RESOURCE_IDENTIFIER_HPP
#define RESOURCE_IDENTIFIER_HPP

#include "ResourceHolder.hpp"

namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID 
	{
		None,
		Eagle,
		Raptor,
		Avenger,
		Bullet,
		Missile,
		Desert,
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TitleScreen,
		ButtonNormal,
		ButtonSelected,
		ButtonPressed,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}
// forward declare
template <typename Identifier, typename Resource>
class ResourceHolder;

typedef ResourceHolder<Textures::ID, sf::Texture> 	TextureHolder;
typedef ResourceHolder<Fonts::ID, sf::Font>			FontHolder;

#endif