#pragma once
#include "EntityInterface.h"
class EntityBall : EntityInterface
{
public:
	EntityBall();
	~EntityBall();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, Vector2 position = Vector2());
	void launch(Vector2 direction);
	void destroy(TransformManager& transformManager) override;
	void update(EntityContainer& entityManager) override;
	void updateDraw() override {};

protected:
	SimpleTransform* transform;
	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;
};

