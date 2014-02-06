#include "LoadingState.hpp"


LoadingState::LoadingState(StateStack& stack, Context context)
	:State(stack, context)
{
	sf::RenderWindow &window = *getContext().window;
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setString("Loading Resources");
	centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));
	mProgressBarBackground.setPosition(10, mLoadingText.getPosition().y + 40);

	mProgressBar.setFillColor(sf::Color(100,100,100));
	mProgressBar.setSize(sf::Vector2f(200, 10));
	mProgressBar.setPosition(10, mLoadingText.getPosition().y + 40);
	
	mAnimatedCircle.setColor(sf::Color(255, 140, 0));
	mAnimatedCircle.setBackgroundColor(sf::Color::Black);
	mAnimatedCircle.setRadius(20.f);
	mAnimatedCircle.setPosition(sf::Vector2f(window.getSize())-sf::Vector2f(2.f*mAnimatedCircle.getRadius(), 2.f*mAnimatedCircle.getRadius()));

	setCompletion(0.f);

	mLoadingTask.execute();
}
void LoadingState::draw()
{
	sf::RenderWindow &window = *getContext().window;
	
	window.setView(window.getDefaultView());
	
	window.draw(mLoadingText);
	window.draw(mProgressBarBackground);
	window.draw(mProgressBar);
	
	window.draw(mAnimatedCircle);
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}

bool LoadingState::update(sf::Time dt)
{
	if(mLoadingTask.isFinished())
	{
		requestStackPop();
		requestStackPush(States::Game);
	}
	else
	{
		setCompletion(mLoadingTask.getCompletion());
		mAnimatedCircle.update(dt);
	}
	
	return true;
}

void LoadingState::setCompletion(float percent)
{
	if(percent > 1.f)
		percent = 1.f;
	
	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x*percent, mProgressBar.getSize().y));
}
