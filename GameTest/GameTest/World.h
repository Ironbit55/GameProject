/*
 * Author: Edward Curran
 * Class: World
 * 
 * World is basically the seperation from the engine framework and the game.
 * this should be abstracted out so that theres a base world class which this should inherit from
 * so the idea is could use the engine at a low level by creating a new World class for each game
 * alternativly I'm going to provide a world implementation which has a gameobject/entity system built on top of it
 * which provides are more managed environment to build a game on top of
 * 
 */


#pragma once
#include "ContentManager.h"
#include "TransformManager.h"
#include "InputReceiver.h"
#include "EntityAttempt/EntityManager.h"
#include "AudioManager.h"

struct FireProjectileMessageData {
	Vector2 position;
	Vector2 direction;
};

struct EntityDef{
	std::string name;
	int entityType;
	std::string textureName;
	Vector2 position;

};


class World : InputReceiver
{
public:
	World(TransformManager& transformManager, ContentManager& contentManager, Camera& camera) : transformManager(transformManager), camera(camera), entityManager(transformManager, contentManager) {};
	~World();


	void loadLevel(const std::wstring& levelFileName);;

	void loadContent(ContentManager& contentManager);

	void initialise();



	void handleInput(InputActors inputActor, MappedInput* mappedInput) override;

	void fireProjectile(Message msg);

	void update(Camera& camera);;
protected:
	void handleWorldInput(MappedInput* mappedInput);

	/*
	 * Helper for attempting to read from a file (or other istream-compatible interface)
	 * i got this from 
	 * http://scribblings-by-apoch.googlecode.com/
	 */
	template <typename OutType>
	OutType attemptRead(std::wistream& stream)
	{
		OutType out;
		if (!(stream >> out))
			throw std::exception("Failed to read a required value");

		return out;
	}

private:
	int dragonTextureId;
	int wallTextureId;
	int raiderTextureId;

	int test = 0;
	TransformManager& transformManager;
	Camera& camera;
	EntityManager entityManager;

};

