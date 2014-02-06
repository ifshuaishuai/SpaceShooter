#include "Pickup.hpp"

#include "DataTables.hpp"

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

int Pickup::Count = 0;

Pickup::Pickup(Type type, const TextureHolder &textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
{
	++Count;

	centerOrigin(mSprite);
}

Pickup::~Pickup()
{
	--Count;
}

// virtual
unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

// virtual
sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft &player) const
{
	Table[mType].action(player);
}

// virtual
void Pickup::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

