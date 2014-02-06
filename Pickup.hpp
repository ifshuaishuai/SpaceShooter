#ifndef PICKUP_HPP
#define PICKUP_HPP
 
#include "Entity.hpp"

#include "Aircraft.hpp"

class Pickup : public Entity
{
public:
	enum Type
	{
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		TypeCount,
	};

	static int Count;
public:
	Pickup(Type type, const TextureHolder &textures);
	~Pickup();

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void apply(Aircraft &player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	Type		mType;
	sf::Sprite 	mSprite;
};

#endif