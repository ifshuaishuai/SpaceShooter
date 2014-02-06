#include "Player.hpp"

#include <iostream>

struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{}

	void operator() (Aircraft &aircraft, sf::Time) const 
	{
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
: mKeyBinding()
, mActionBinding()
, mMissionStatus(MissionStatus::MissionRunning)
{
	mKeyBinding[sf::Keyboard::Left] 	= MoveLeft;
	mKeyBinding[sf::Keyboard::Right] 	= MoveRight;
	mKeyBinding[sf::Keyboard::Up] 		= MoveUp;
	mKeyBinding[sf::Keyboard::Down] 	= MoveDown;
	mKeyBinding[sf::Keyboard::M]		= LaunchMissile;
	mKeyBinding[sf::Keyboard::Space]	= Fire;

	initializeActions();

	for(auto &pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commands)
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		Command output;
		output.category = Category::PlayerAircraft;
		output.action = [] (SceneNode &s, sf::Time)
		{
			std::cout << s.getPosition().x << ", " << s.getPosition().y << std::endl;
		};

		commands.push(output);
	}

	if(event.type == sf::Event::KeyPressed)
	{
		auto found = mKeyBinding.find(event.key.code);
		if(found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue &commands)
{
	for(auto pair : mKeyBinding)
	{
		if(sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	auto itr = mKeyBinding.begin();
	for(; itr!=mKeyBinding.end(); ++itr)
	{
		if(itr->second == action)	// find action, requires only 1 key binds to 1 action
			break;
	}

	mKeyBinding.erase(itr);
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action)
{
	for(auto &pair : mKeyBinding)
		if(pair.second == action)
			return pair.first;

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	mMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mMissionStatus;
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;

	mActionBinding[MoveLeft].action	 		= derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action 		= derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action    		= derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action  		= derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));

	mActionBinding[Fire].action 			= derivedAction<Aircraft>([] (Aircraft &a, sf::Time){ a.fire();});
	mActionBinding[LaunchMissile].action 	= derivedAction<Aircraft>([](Aircraft &a, sf::Time){a.launchMissile();});
}

bool Player::isRealtimeAction(Action action)
{
	switch(action)
	{
		case MoveUp:
		case MoveDown:
		case MoveLeft:
		case MoveRight:
		case Fire:
		return true;

		default:
		return false;
	}
}
