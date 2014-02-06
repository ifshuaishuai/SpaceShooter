#include "SceneNode.hpp"

#include <SFML/Graphics.hpp>

SceneNode::SceneNode(Category::Type category)
: mChildren()
, mParent()
, mDefaultCategory(category)
{}

void SceneNode::attachChild(SceneNode::Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), 
		[&] (Ptr &p) -> bool {return p.get() == &node;});
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue &commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::onCommand(const Command &command, sf::Time dt)
{
	if(command.category & getCategory())
		command.action(*this, dt);

	for(Ptr &child : mChildren)
		child->onCommand(command, dt);
}

void SceneNode::checkNodeCollision(SceneNode &node, std::set<Pair> &collisionPairs)
{
	if(this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for(Ptr &child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::set<Pair> &collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for(Ptr &child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

// virtual, and should not be override by subclass, final function
void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	for(auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
		(*itr)->draw(target, states);

//	drawBoundingRect(target, states);
}

// virtual, should be override
void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{}

// virtual
void SceneNode::updateCurrent(sf::Time dt, CommandQueue &commands)
{
	// do nothing
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue &commands)
{
	for(Ptr &child : mChildren)
		child->update(dt, commands);
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for(const SceneNode *node=this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;
	return transform;
}

//virtual
unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}


void SceneNode::removeWrecks()
{
	// Remove all children which request so
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	// Call function recursively for all remaining children
	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::isDestroyed() const
{
	return false;	
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

void SceneNode::drawBoundingRect(sf::RenderTarget &target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}
