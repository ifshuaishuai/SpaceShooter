#include "Aircraft.hpp"

#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"

#include <cmath>

#include <iostream>

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

Textures::ID toTextureID(Aircraft::Type type)
{
	switch(type)
	{
		case Aircraft::Eagle:
		return Textures::Eagle;

		case Aircraft::Raptor:
		return Textures::Raptor;

		case Aircraft::Avenger:
		return Textures::Avenger;

		default:
		return Textures::None;
	}
	return Textures::None;
}

int Aircraft::Count = 0;

Aircraft::Aircraft(Type type, const TextureHolder &textures, const FontHolder &fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(toTextureID(type)))
, mHealthDisplay(nullptr)
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mIsFiring(false)
, mIsLaunchingMissile(false)
, mMissileAmmo(3)
, mFireCountdown(sf::Time::Zero)
, mFireRateLevel(1)
, mFireCommand()
, mMissileCommand()
, mDropPickupCommand()
, mSpreadLevel(1)
, mIsMarkedForRemoval(false)
{
	++Count;

	if(!isAllied())
		mSpreadLevel = 3;

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width/2.f, bounds.height/2.f);

	mFireCommand.category = Category::SceneAirLayer;
	mFireCommand.action = [this, &textures] (SceneNode &node, sf::Time)
	{
		createBullets(node, textures);
	};

	mMissileCommand.category = Category::SceneAirLayer;
	mMissileCommand.action = [this, &textures] (SceneNode &node, sf::Time)
	{
		createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
	};

	mDropPickupCommand.category = Category::SceneAirLayer;
	mDropPickupCommand.action = [this, &textures] (SceneNode &node, sf::Time) 
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> helthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = helthDisplay.get();
	attachChild(std::move(helthDisplay));
}

Aircraft::~Aircraft()
{
	--Count;
}

// virtual
void Aircraft::updateCurrent(sf::Time dt, CommandQueue &commands)
{
	if(isDestroyed())
	{
		checkPickupDrop(commands);

		mIsMarkedForRemoval = true;
		return;
	}
	
	checkProjectileLaunch(dt, commands);

	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);	// must call this, or the aircraft won't move!!!

	updateTexts();
}

// virtual
void Aircraft::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

// virtual
unsigned int Aircraft::getCategory() const
{
	switch(mType)
	{
		case Aircraft::Eagle:
		return Category::PlayerAircraft;
		break;

		default:
		return Category::EnemyAircraft;
	}
}

float Aircraft::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Aircraft::fire()
{
	if(Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Aircraft::launchMissile()
{
	if(mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}

void Aircraft::updateTexts()
{
	mHealthDisplay->setString("HP:" + std::to_string(getHitpoints()));
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction> &directions = Table[mType].directions;
	if(!directions.empty())
	{
		float distanceToTravel = directions[mDirectionIndex].distance;
		if(mTravelledDistance > distanceToTravel)
		{
			mTravelledDistance = 0.f;
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
		}
		float radians = degreeToRadian(directions[mDirectionIndex].angle + 90);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);
		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue &commands)
{
	if(!isAllied())
		fire();

	if(mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if(mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	if(mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
	}
}

void Aircraft::checkPickupDrop(CommandQueue &commands)
{
	if(!isAllied() && randomInt(3) == 0)
		commands.push(mDropPickupCommand);
}

void Aircraft::createBullets(SceneNode &node, const TextureHolder &textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch(mSpreadLevel)
	{
		case 1:
			createProjectile(node, type, 0.0f, 0.5f, textures);
			break;

		case 2:
			createProjectile(node, type, -0.33f, 0.33f, textures);
			createProjectile(node, type, +0.33f, 0.33f, textures);
			break;

		case 3:
			createProjectile(node, type, -0.5f, 0.33f, textures);
			createProjectile(node, type,  0.0f, 0.5f, textures);
			createProjectile(node, type, +0.5f, 0.33f, textures);
			break;
	}	
}


void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(SceneNode &node, const TextureHolder &textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));	
}

bool Aircraft::isAllied() const
{
	return mType == Eagle;
}

bool Aircraft::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

sf::FloatRect Aircraft::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Aircraft::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;	
}

void Aircraft::increaseSpread()
{
	mSpreadLevel++;
	if(mSpreadLevel > 3)
		mSpreadLevel = 3;
}

void Aircraft::increaseFireRate()
{
	mFireRateLevel++;
}
