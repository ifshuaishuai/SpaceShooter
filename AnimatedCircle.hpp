#ifndef ANIMATED_CIRCLE_HPP
#define ANIMATED_CIRCLE_HPP

#include <SFML/Graphics.hpp>

class AnimatedCircle : public sf::Drawable
{
public:
	explicit AnimatedCircle(const sf::Vector2f &the_position = sf::Vector2f(), const float the_radius = 0.f, const sf::Color &the_color = sf::Color());
	void update(sf::Time dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	void setPosition(const sf::Vector2f &position);
	sf::Vector2f getPosition() const;
	
	void setRadius(float radius);
	float getRadius() const;
	
	void setColor(const sf::Color &color);
	sf::Color getColor() const;
	
	void setBackgroundColor(const sf::Color &backgroundColor);
	sf::Color getBackgroundColor() const;
	
private:
	void setAllStuff();
	
private:
	sf::CircleShape 	outerCircle;
	sf::CircleShape		innerCircle;
	sf::RectangleShape	spinRect;
	
	sf::Vector2f		position;
	float				radius;
	sf::Color			color;
	sf::Color			backgroundColor;
	
	static const sf::Time TimePerDegree;
};

#endif
