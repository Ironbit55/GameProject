#pragma once
#include "../GameTest/ContentManager.h"
#include "../GameTest/TransformManager.h"
#include "GameTest/InputReceiver.h"


class EntityContainer;
enum EntityType;

struct EntityDef {
	std::string name;
	EntityType entityType;
	std::string textureName;
	Vector2 position;
	Vector2 scale;
	Vector4 colour = Vector4(1, 1, 1, 1);
	float rotation;
	InputActors inputActor;
	int number;
	bool spawn;


};

class EntityInterface
{
public:
	EntityInterface() {}
	virtual ~EntityInterface() {};


	//virtual void loadContent(ContentManager contentManager);

	void initialise(ContentManager& contentManager, TransformManager& transformManager) {};

	//destory gets called by entity manager
	//entity should use destroy method to to destroy any physics, render or transform components
	//it holds, it can then be destructed as normal.
	virtual void destroy(TransformManager& transformManager) {}

	//to use this entity manager, inherting entities will have to include EntityManager.h in their .cpp file
	virtual void update(float msec, EntityContainer& entityManager) = 0;

	virtual void updateDraw() {};

	virtual EntityType getEntityType() = 0;


protected:
	//EntityInterface* entityId;
	//EntityType entityType;


};

