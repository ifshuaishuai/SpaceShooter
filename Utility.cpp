#include "Utility.hpp"

#include <cassert>
#include <random>
#include <ctime>

const float PI = 3.141592653589793238462643383f;

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

void centerOrigin(sf::Sprite &sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(sf::Text &text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

float degreeToRadian(float degree)
{
	// 180 degree = 1 PI
	return PI * (degree / 180); 
}

float radianToDegree(float radian)
{
	return 180 * (radian / PI);
}

float length(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
	assert(vector != sf::Vector2f(0.f, 0.f));
	return vector / length(vector);
}

int randomInt(int range)
{
	std::uniform_int_distribution<> distr(0, range - 1);
	return distr(RandomEngine);
}
