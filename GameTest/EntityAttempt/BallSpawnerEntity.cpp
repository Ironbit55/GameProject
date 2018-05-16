#include "BallSpawnerEntity.h"
#include "EntityManager.h"


BallSpawnerEntity::BallSpawnerEntity() {
	addListener(MESSAGE_RESPAWN_BALL);
}


BallSpawnerEntity::~BallSpawnerEntity()
{
}

void BallSpawnerEntity::initialise(ContentManager& contentManager, TransformManager& transformManager,
	EntityDef& entityDef) {
	position = entityDef.position;
	playerIndex = entityDef.inputActor;
	if(entityDef.spawn){
		spawnBall();
	}
}

void BallSpawnerEntity::update(float msec, EntityContainer& entityManager) {
}

EntityType BallSpawnerEntity::getEntityType() {
	return ENTITY_BALL_SPAWNER;
}

void BallSpawnerEntity::spawnBall() {
	float delay = 0.0f;


	CreateEntityMsgData msgData;
	msgData.entityDef = EntityDef();
	msgData.entityDef.entityType = ENTITY_BALL;
	msgData.entityDef.position = position;
	msgData.entityDef.inputActor = InputManager::asInputActor(playerIndex);

	if(playerIndex == 0){
		msgData.entityDef.colour = Vector4(1, 0, 0, 1);
	}else if(playerIndex == 1) {
		msgData.entityDef.colour = Vector4(0, 1, 0, 1);
	}

	Message msg;
	msg.messageType = MESSAGE_CREATE_ENTITY;
	msg.timeUntillDispatch = delay;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);
	MessagingService::instance().pushMessage(msg);
}

void BallSpawnerEntity::onReceiveMessage(Message& msg) {
	switch (msg.messageType)
	{
	case MESSAGE_RESPAWN_BALL: {
		RepsawnBallMsgData* msgData = static_cast<RepsawnBallMsgData*>(msg.dataPayload);
		if(msgData->owningPlayer == playerIndex) {
			spawnBall();
		}
		break;
	}

	default:
		break;
	}
}
