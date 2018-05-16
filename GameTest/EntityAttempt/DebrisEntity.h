#pragma once
#include "EntityInterface.h"
class DebrisEntity : EntityInterface
{
public:
	DebrisEntity();
	~DebrisEntity();

	void initialise(ContentManager& contentManager, TransformManager &transformManager, Vector2 position = Vector2(), Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	
	void destroy(TransformManager& transformManager) override;
	void update(float msec, EntityContainer& entityManager) override;
	void updateDraw() override {};
	EntityType getEntityType() override;

protected:
	SimpleTransform* transform;
	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;

};

