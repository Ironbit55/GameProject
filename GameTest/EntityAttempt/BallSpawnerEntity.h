#pragma once
#include "EntityInterface.h"

class BallSpawnerEntity : EntityInterface, MessageReceiver
{
public:
	BallSpawnerEntity();
	~BallSpawnerEntity();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef& entityDef);
	void update(float msec, EntityContainer& entityManager) override;
	EntityType getEntityType() override;
	void spawnBall();


protected:
	void onReceiveMessage(Message& msg) override;
	int playerIndex;
	Vector2 position;
};

