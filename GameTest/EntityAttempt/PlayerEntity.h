#pragma once
#include "GameTest/InputReceiver.h"
#include "PhysicsEntity.h"

class PlayerEntity : public PhysicsEntity, public InputReceiver
{
public:
	PlayerEntity();
	~PlayerEntity() override;

	void initialise(ContentManager& contentManager, TransformManager &transformManager, InputActors inputActor, Vector2 position = Vector2(), float rotation = 0);

	void destroy(TransformManager& transformManager) override;
	void update(float msec, EntityContainer& entityManager) override;
	void updateDraw() override {};

	void handleInput(InputActors inputActor, MappedInput* mappedInput) override;
	void shootBall(Vector2 dir);
	void shootProjectile();

	EntityType getEntityType() override;


	bool acceptsContacts() override;
	void onContactBegin(const ContactData& data) override;
	void onContactEnd(const ContactData& data) override;
	void onContactPreSolve(const ContactData& data) override;
protected:
	int numBalls = 0;
	int numProjectiles = 0;
	InputActors owningPlayer;
	int ballOwners[4];
	int onGround = false;
	int jumpCharges;

};

