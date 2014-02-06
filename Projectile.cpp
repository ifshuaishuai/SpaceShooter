#include "Projectile.hpp"

#include <iostream>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

int Projectile::Count = 0;

Projectile::Projectile(Type type, const TextureHolder &textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTargetDirection()
{
	centerOrigin(mSprite);
	if(getCategory() == Category::EnemyProjectile)
		mSprite.rotate(180);

	++Count;
}

Projectile::~Projectile()
{
	--Count;
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return mType == Missile;
}

// virtual 
unsigned int Projectile::getCategory() const
{
	if(mType == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

//	virtual 
sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

int Projectile::getDamage() const
{
	return Table[mType].damage;
}


// virtual 
void Projectile::updateCurrent(sf::Time dt, CommandQueue &commands)
{
	if(isGuided())
	{
		const float approachRate = 200.f;	// 转角速度

		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(radianToDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}

	Entity::updateCurrent(dt, commands);
}

// virtual
void Projectile::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}


