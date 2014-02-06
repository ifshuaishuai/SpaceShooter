#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
 
#include "Entity.hpp"
#include "ResourceIdentifier.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"

#include <SFML/Graphics.hpp>

class Projectile : public Entity
{
public:
	enum Type
	{
		AlliedBullet,
		EnemyBullet,
		Missile,
		TypeCount
	};

	static int Count;
public:
	Projectile(Type type, const TextureHolder &textures);
	~Projectile();

	void guideTowards(sf::Vector2f position);
	bool isGuided() const;

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	float getMaxSpeed() const;
	int getDamage() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	Type 			mType;
	sf::Sprite 		mSprite;
	sf::Vector2f 	mTargetDirection;
};
 
#endif