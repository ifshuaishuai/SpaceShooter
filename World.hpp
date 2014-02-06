#ifndef WORLD_HPP
#define WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
 
class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow &window, FontHolder &fonts);

	void update(sf::Time dt);
	void draw();

	CommandQueue & getCommandQueue();

	bool hasAlivePlayer() const;
	bool hasPlayerReachedEnd() const;
	
private:
	void loadTextures();
	void buildScene();

	void adaptPlayerVelocity();
	void adaptPlayerPosition();

	sf::FloatRect getViewBounds() const;
	sf::FloatRect getBattlefieldBounds() const;

	void spawnEnemies();
	void addEnemies();
	void addEnemy(Aircraft::Type type, float x, float y);

	void guideMissiles();
	void destroyEntitiesOutsideView();

	void handleCollisions();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount,
	};

	struct SpawnPoint
	{
		SpawnPoint(Aircraft::Type type, float x, float y)
		: type(type)
		, x(x)
		, y(y)
		{}

		Aircraft::Type type;
		float x;
		float y;
	};

private:
	sf::RenderWindow 		&mWindow;
	sf::View				mWorldView;
	TextureHolder			mTextures;
	FontHolder				&mFonts;
	SceneNode				mSceneGraph;
	std::array<SceneNode *, LayerCount> mSceneLayers;

	sf::FloatRect			mWorldBounds;
	sf::Vector2f			mSpawnPosition;
	float 					mScrollSpeed;
	Aircraft				*mPlayerAircraft;	

	CommandQueue			mCommandQueue;

	std::vector<SpawnPoint> mEnemySpawnPoints;

	std::vector<Aircraft *> mActiveEnemies;
};
 
bool matchesCategories(SceneNode::Pair &colliders, Category::Type type1, Category::Type type2);

#endif