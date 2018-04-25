#pragma once
#include "EntityInterface.h"
#include "GameTest/InputReceiver.h"

class WallEntity : public EntityInterface {
public:
	WallEntity() {};
	~WallEntity() {};

	//void loadContent(ContentManager contentManager) override;

	void initialise(ContentManager& contentManager, TransformManager &transformManager, Vector2 position = Vector2(), float rotation = 0);

	void destroy(TransformManager& transformManager) override;

	void update(EntityContainer& entityManager) override;

	void updateDraw() override;

private:
	SimpleTransform* transform;
	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;


};

