#ifndef DATA_TABLES_HPP
#define DATA_TABLES_HPP

#include "ResourceIdentifier.hpp"

#include <vector>

#include <SFML/System/Time.hpp>

struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{}

	float angle;
	float distance;
};

struct AircraftData
{
	int 			hitpoints;
	float 			speed;
	Textures::ID 	texture;
	sf::Time		fireInterval;

	std::vector<Direction> directions;
};

struct ProjectileData
{
	int 			damage;
	float			speed;
	Textures::ID 	texture;
};

class Aircraft;

struct PickupData
{
	std::function<void (Aircraft &)> 	action;
	Textures::ID 						texture;
};

std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();

#endif