#pragma once
#include "EntityInterface.h"
#include "GameTest/InputReceiver.h"

class PlayerEntity : public EntityInterface, public InputReceiver
{
public:
	PlayerEntity();
	~PlayerEntity() override;

	void initialise(ContentManager& contentManager, TransformManager &transformManager, InputActors inputActor, Vector2 position = Vector2(), float rotation = 0);

	void destroy(TransformManager& transformManager) override;
	void update(EntityContainer& entityManager) override;
	void updateDraw() override {};

	void handleInput(InputActors inputActor, MappedInput* mappedInput) override;
protected:
	SimpleTransform* transform;
	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;

};

