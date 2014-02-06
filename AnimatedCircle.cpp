#include "AnimatedCircle.hpp"

const sf::Time AnimatedCircle::TimePerDegree = sf::seconds(3.f/360.f);

AnimatedCircle::AnimatedCircle(const sf::Vector2f &the_position, const float the_radius, const sf::Color &the_color)
	:outerCircle()
	,innerCircle()
	,spinRect()
	,position(the_position)
	,radius(the_radius)
	,color(the_color)
{
	setAllStuff();
}

void AnimatedCircle::update(sf::Time dt)
{
	double degree = dt / TimePerDegree;
	
	spinRect.rotate(degree);
}

// virtual
void AnimatedCircle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(outerCircle, states);
	target.draw(innerCircle, states);
	target.draw(spinRect, states);
}

void AnimatedCircle::setPosition(const sf::Vector2f &position)
{
	this->position = position;
	setAllStuff();
}
sf::Vector2f AnimatedCircle::getPosition() const
{
	return this->position;
}

void AnimatedCircle::setRadius(float radius)
{
	this->radius = radius;
	setAllStuff();
}
float AnimatedCircle::getRadius() const
{
	return this->radius;
}

void AnimatedCircle::setColor(const sf::Color &color)
{
	this->color = color;
	setAllStuff();
}
sf::Color AnimatedCircle::getColor() const
{
	return this->color;
}


void AnimatedCircle::setBackgroundColor(const sf::Color &backgroundColor)
{
	this->backgroundColor = backgroundColor;
	setAllStuff();
}
sf::Color AnimatedCircle::getBackgroundColor() const
{
	return this->backgroundColor;
}

void AnimatedCircle::setAllStuff()
{
	outerCircle.setPosition(position);
	outerCircle.setRadius(radius);
	outerCircle.setFillColor(color);

	const float innerPercent = 0.3f;
	innerCircle.setPosition(position+sf::Vector2f(innerPercent*radius, innerPercent*radius));
	innerCircle.setRadius((1-innerPercent) * radius);
	innerCircle.setFillColor(backgroundColor);
	
	const float rectPercent = 0.25f;
	spinRect.setOrigin(rectPercent/2.f*radius, radius);	// origin原来是相对与spinRect左上角为(0,0),偏移到中心
	spinRect.setPosition(position+sf::Vector2f((1-rectPercent/2.f)*radius, 0.f)+spinRect.getOrigin());
	spinRect.setSize(sf::Vector2f(rectPercent*radius, 2.f*radius));
	spinRect.setFillColor(backgroundColor);
}