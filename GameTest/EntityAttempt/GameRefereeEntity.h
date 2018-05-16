#pragma once
#include "EntityInterface.h"

struct ScoreGoalMsgData {
	int PlayerIndex;
};
class GameRefereeEntity : EntityInterface, MessageReceiver
{
public:
	GameRefereeEntity();
	~GameRefereeEntity();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef& entityDef);
	void update(float msec, EntityContainer& entityManager) override;
	EntityType getEntityType() override;
	void onReceiveMessage(Message& msg) override;
protected:
	float time = 60000 * 5;
	float previousTime = 60000 * 5;
	int player1Score;
	int player2Score;
};

