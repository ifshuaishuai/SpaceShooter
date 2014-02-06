#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP
 
#include "Entity.hpp"
#include "ResourceIdentifier.hpp"
#include "TextNode.hpp"
#include "Projectile.hpp"

#include <SFML/Graphics.hpp>

class Aircraft : public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
		Avenger,
		TypeCount,
	};

	static int Count;

public:
	explicit Aircraft(Type type, const TextureHolder &textures, const FontHolder &fonts);
	~Aircraft();
	
	virtual void updateCurrent(sf::Time dt, CommandQueue &commands);

	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	virtual unsigned int getCategory() const;

	float getMaxSpeed() const;
	
	void fire();
	void launchMissile();

	virtual sf::FloatRect getBoundingRect() const;

	void collectMissiles(unsigned int count);
	void increaseSpread();
	void increaseFireRate();

	bool isMarkedForRemoval() const;
	
private:
	void updateTexts();
	void updateMovementPattern(sf::Time dt);

	void checkProjectileLaunch(sf::Time dt, CommandQueue &commands);
	void checkPickupDrop(CommandQueue &commands);


	void createBullets(SceneNode &node, const TextureHolder &textures) const;
	void createProjectile(SceneNode &node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder &textures) const;
	void createPickup(SceneNode &node, const TextureHolder &textures) const;

	bool isAllied() const;

private:
	Type 		mType;
	sf::Sprite 	mSprite;

	TextNode	*mHealthDisplay;

	float 		mTravelledDistance;
	int 		mDirectionIndex;

	bool		mIsFiring;
	bool 		mIsLaunchingMissile;
	int 		mMissileAmmo;

	sf::Time 	mFireCountdown;
	int 		mFireRateLevel;
	Command 	mFireCommand;
	Command 	mMissileCommand;
	Command 	mDropPickupCommand;

	int 		mSpreadLevel;

	bool 		mIsMarkedForRemoval;
};

#endif