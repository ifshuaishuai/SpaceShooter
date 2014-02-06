#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <cassert>
 
#include "Category.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode *, SceneNode *> Pair;

public:
	SceneNode(Category::Type category = Category::None);

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode &node);

	void update(sf::Time dt, CommandQueue &commands);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

	virtual unsigned int getCategory() const;
	void onCommand(const Command &command, sf::Time dt);

	void removeWrecks();
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isDestroyed() const;
	virtual bool isMarkedForRemoval() const;

	void checkNodeCollision(SceneNode &node, std::set<Pair> &collisionPairs);
	void checkSceneCollision(SceneNode &sceneGraph, std::set<Pair> &collisionPairs);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

	virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
	void updateChildren(sf::Time dt, CommandQueue &commands);

	void drawBoundingRect(sf::RenderTarget &target, sf::RenderStates) const;
private:
	std::vector< Ptr > 	mChildren;
	SceneNode 			*mParent;
	Category::Type 		mDefaultCategory;
};

inline float distance(const SceneNode &lhs, const SceneNode &rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}

inline bool collision(const SceneNode &lhs, const SceneNode &rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

#endif