#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

// forward declaration
class SceneNode;

struct Command
{
	Command();
	
	std::function<void (SceneNode &, sf::Time)> action;
	unsigned int								category;
};

template <typename GameObject, typename Function>
std::function<void (SceneNode &, sf::Time)> derivedAction(Function fn)
{
	return [=] (SceneNode &node, sf::Time dt)
	{
		assert(dynamic_cast<GameObject *>(&node) != nullptr);

		fn(static_cast<GameObject &>(node), dt);
	};
}
 
#endif