#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	explicit Entity(int hitpoints);

	void repair(int hitpoints);
	void damage(int hitpoints);
	void destroy();

	int getHitpoints() const;
	bool isDestroyed() const;
	
	void setVelocity(float x, float y);
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity();

	void accelerate(sf::Vector2f vel);
	void accelerate(float vx, float vy);

	virtual void updateCurrent(sf::Time dt, CommandQueue &);
	
private:
	sf::Vector2f	mVelocity;
	int				mHitpoints;
};

 
#endif