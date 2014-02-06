#include "World.hpp"

#include <limits>
#include <iostream>

#include "Pickup.hpp"

World::World(sf::RenderWindow &window, FontHolder &fonts)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mFonts(fonts)
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
, mCommandQueue()
, mEnemySpawnPoints()
, mActiveEnemies()
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// this is GameController!!!
	mWorldView.move(0, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();
	guideMissiles();

	while(!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	handleCollisions();
	mSceneGraph.removeWrecks();
	spawnEnemies();

	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition(); // last adapt position, not call mSceneGraph update, or the position will be wrong if hold one key down!!!

	if(hasPlayerReachedEnd())
	{
		mWorldView.setCenter(mSpawnPosition);
		mPlayerAircraft->setPosition(mSpawnPosition);

		mPlayerAircraft->collectMissiles(100);
		addEnemies();
	}
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue & World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(Textures::Avenger, "Media/Textures/Avenger.png");
	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");

	mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");
	mTextures.load(Textures::Missile, "Media/Textures/Missile.png");

	mTextures.load(Textures::HealthRefill, "Media/Textures/HealthRefill.png");
	mTextures.load(Textures::MissileRefill, "Media/Textures/MissileRefill.png");
	mTextures.load(Textures::FireSpread, "Media/Textures/FireSpread.png");
	mTextures.load(Textures::FireRate, "Media/Textures/FireRate.png");
}

void World::buildScene()
{
	for(std::size_t i=0; i<LayerCount; ++i)
	{
		Category::Type category = (i == Air) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture &texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(100.f, mScrollSpeed);
	mSceneLayers[Air]->attachChild(std::move(leader));

	addEnemies();
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	if(velocity.x != 0.f && velocity.y != 0.f)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize()/2.f, mWorldView.getSize());
	const float borderDistance = 40.f;
	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left+borderDistance);
	position.x = std::min(position.x, viewBounds.left+viewBounds.width-borderDistance);
	position.y = std::max(position.y, viewBounds.top+borderDistance);
	position.y = std::min(position.y, viewBounds.top+viewBounds.height-borderDistance);

	mPlayerAircraft->setPosition(position);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize()/2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::spawnEnemies()
{
	while(!mEnemySpawnPoints.empty() && 
		mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);

		mSceneLayers[Air]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();
	}
}

void World::addEnemies()
{
	addEnemy(Aircraft::Raptor,    0.f,  500.f);
	addEnemy(Aircraft::Raptor,    0.f, 1000.f);
	addEnemy(Aircraft::Raptor, +100.f, 1100.f);
	addEnemy(Aircraft::Raptor, -100.f, 1100.f);
	addEnemy(Aircraft::Avenger, -70.f, 1400.f);
	addEnemy(Aircraft::Avenger, -70.f, 1600.f);
	addEnemy(Aircraft::Avenger,  70.f, 1400.f);
	addEnemy(Aircraft::Avenger,  70.f, 1600.f);
	
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), 
		[](SpawnPoint lhs, SpawnPoint rhs){ return lhs.y < rhs.y;});
}

void World::addEnemy(Aircraft::Type type, float x, float y)
{
	SpawnPoint spawn(type, mSpawnPosition.x + x, mSpawnPosition.y - y);
	mEnemySpawnPoints.push_back(spawn);
}

void World::guideMissiles()
{
	Command enemyCollector;
	enemyCollector.category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft &enemy, sf::Time)
		{
			if(!enemy.isDestroyed())
				mActiveEnemies.push_back(&enemy);
		});

	Command missileGuider;
	missileGuider.category = Category::AlliedProjectile;
	missileGuider.action = derivedAction<Projectile>([this] (Projectile &missile, sf::Time) 
		{
			if(!missile.isGuided())
				return;

			float minDistance = std::numeric_limits<float>::max();
			Aircraft *closestEnemy = nullptr;

			for(Aircraft *enemy : mActiveEnemies)
			{
				float enemyDistance = distance(missile, *enemy);
				if(enemyDistance < minDistance)
				{
					minDistance = enemyDistance;
					closestEnemy = enemy;
				}
			}

			if(closestEnemy)
				missile.guideTowards(closestEnemy->getWorldPosition());
		});

	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyAircraft | Category::Pickup;
	command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
}

bool matchesCategories(SceneNode::Pair &colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if((type1 & category1) && (type2 & category2))
		return true;
	else if((type1 & category2) && (type2 & category1))
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
		return false; 
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for(SceneNode::Pair pair : collisionPairs)
	{
		if(matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto &aircraft = static_cast<Aircraft &>(*pair.first);
			auto &enemy = static_cast<Aircraft &>(*pair.second);

			aircraft.damage(enemy.getHitpoints());
			enemy.destroy();
		}
		else if(matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))	
		{
			auto &aircraft = static_cast<Aircraft &>(*pair.first);
			auto &pickup = static_cast<Pickup &>(*pair.second);

			pickup.apply(aircraft);
			pickup.destroy();
		}
		else if(matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
		{
			auto &aircraft = static_cast<Aircraft &>(*pair.first);
			auto &projectile = static_cast<Projectile &>(*pair.second);

			aircraft.damage(projectile.getDamage());
			projectile.destroy();
		}
		else if(matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile))
		{
			auto &enemy = static_cast<Aircraft &>(*pair.first);
			auto &projectile = static_cast<Projectile &>(*pair.second);

			enemy.damage(projectile.getDamage());
			projectile.destroy();
		}
		else if(matchesCategories(pair, Category::EnemyProjectile, Category::AlliedProjectile))
		{
			// 子弹碰撞
			auto &enemyProjectile = static_cast<Projectile &>(*pair.first);
			auto &alliedProjectile = static_cast<Projectile &>(*pair.second);

			enemyProjectile.destroy();
			alliedProjectile.destroy();
		}
	}
}

bool World::hasAlivePlayer() const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

