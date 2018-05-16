#pragma once
#include "PhysicsEntity.h"

struct RepsawnBallMsgData {
	int owningPlayer;
};

class EntityBall : public PhysicsEntity
{
public:
	EntityBall();
	~EntityBall();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef entityDef);
	void launch(Vector2 direction, float magnitudeScale);
	void launch(Vector2 position, Vector2 direction, float magnitudeScale);
	void update(float msec, EntityContainer& entityManager) override;
	void updateDraw() {};

	EntityType getEntityType() override;


	bool acceptsContacts() override;
	void onContactBegin(const ContactData& data) override;
	void onContactEnd(const ContactData& data) override;

	int getOriginalOwner(){
		return originalPlayerOwner;
	}
	int getLastTouchedPlayer(){
		return lastTouchedByPlayer;
	}
	void setTouchedByPlayer(int playerIndex) {
		lastTouchedByPlayer = playerIndex;
	}

	

	void doReset(float delay = 5000.0f){
		if (!toDelete) {
			toDelete = true;
			spawnBall(delay);
		}
	}

	void spawnBall(float delay = 5000.0f);
protected:
	int lastTouchedByPlayer;
	int originalPlayerOwner;
	bool reset;
	bool toDelete;
	
};

