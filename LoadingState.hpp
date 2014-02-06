#ifndef LOADING_STATE_HPP
#define LOADING_STATE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.hpp"
#include "ParallelTask.hpp"

#include "AnimatedCircle.hpp"

class LoadingState : public State
{
public:
	LoadingState(StateStack &stack, Context context);
	
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);
	
	void setCompletion(float percent);

private:
	sf::Text			mLoadingText;
	sf::RectangleShape	mProgressBarBackground;
	sf::RectangleShape	mProgressBar;
	
	AnimatedCircle		mAnimatedCircle;
	
	ParallelTask		mLoadingTask;
};

#endif