#include "Entity.hpp"

#include <iostream>

Entity::Entity(int hitpoints)
: mHitpoints(hitpoints)
{}

void Entity::repair(int hitpoints)
{
	mHitpoints += hitpoints;
}

void Entity::damage(int hitpoints)
{
	mHitpoints -= hitpoints;
}

void Entity::destroy()
{
	mHitpoints = 0;
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

bool Entity::isDestroyed() const
{
	return mHitpoints <= 0;
}

	
void Entity::setVelocity(float x, float y)
{
	mVelocity.x = x;
	mVelocity.y = y;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

sf::Vector2f Entity::getVelocity()
{
	return mVelocity;
}

void Entity::accelerate(sf::Vector2f vel)
{
	mVelocity += vel;	
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;	
}

// move a Entity's position
void Entity::updateCurrent(sf::Time dt, CommandQueue &)
{
	move(mVelocity * dt.asSeconds());
}
