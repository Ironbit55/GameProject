#include "GameRefereeEntity.h"
#include "EntityManager.h"


GameRefereeEntity::GameRefereeEntity() {
	addListener(MessageType::MESSAGE_SCORE_GOAL);
}


GameRefereeEntity::~GameRefereeEntity()
{
}

void GameRefereeEntity::initialise(ContentManager& contentManager, TransformManager& transformManager,
	EntityDef& entityDef) {
	player1Score = 0;
	player2Score = 0;
}

void GameRefereeEntity::update(float msec, EntityContainer& entityManager) {
	time -= msec;

	if(previousTime - time > 10000){
		previousTime = time;

		cout << "time: " << (int)(time / 1000) << "\n";
		cout << "player 1 score: " << player1Score << "\n";
		cout << "player 2 score: " << player2Score << "\n\n";
	}
}

EntityType GameRefereeEntity::getEntityType() {
	return ENTITY_INVALID;
}

void GameRefereeEntity::onReceiveMessage(Message& msg) {
	switch (msg.messageType)
	{
	case MESSAGE_SCORE_GOAL: {
		ScoreGoalMsgData* msgData = static_cast<ScoreGoalMsgData*>(msg.dataPayload);
		if(msgData->PlayerIndex == 0){
			player1Score++;
		}else if(msgData->PlayerIndex == 1){
			player2Score++;
		}
		break;
	}

	default:
		break;
	}
}
