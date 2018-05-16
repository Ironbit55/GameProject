#pragma once
#include "PhysicsEntity.h"

class GoalEntity : public PhysicsEntity
{
public:
	GoalEntity();
	~GoalEntity();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef& entityDef);


	void update(float msec, EntityContainer& entityManager) override;


	EntityType getEntityType() override;

	bool acceptsContacts() override;
	void onContactBegin(const ContactData& data) override;
	void onContactEnd(const ContactData& data) override;

	void sendGoalMessage(int playerIndex);
	int getPlayer(){
		return playerIndexCanScoreIn;
	}
protected:
	int playerIndexCanScoreIn;
};

