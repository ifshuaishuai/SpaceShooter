#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Aircraft.hpp"
#include "CommandQueue.hpp"
#include "Category.hpp"

#include <SFML/Window/Event.hpp>

#include <map>
#include <algorithm>
#include <string>
#include <iostream>

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		ActionCount,
	};

	enum MissionStatus
	{
		MissionFailure,
		MissionSuccess,
		MissionRunning,
	};

public:
	Player();

	void handleEvent(const sf::Event &event, CommandQueue &commands);
	void handleRealtimeInput(CommandQueue &commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action);
	
	void setMissionStatus(MissionStatus status);
	Player::MissionStatus getMissionStatus() const;
private:
	void initializeActions();
	static bool isRealtimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action> 	mKeyBinding;
	std::map<Action, Command>				mActionBinding;

	MissionStatus 							mMissionStatus;
};
 
#endif