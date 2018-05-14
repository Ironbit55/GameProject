#include "EntityBall.h"



EntityBall::EntityBall()
{
}


EntityBall::~EntityBall()
{
}

void EntityBall::initialise(ContentManager& contentManager, TransformManager& transformManager, Vector2 position) {

	int ballTextureId = contentManager.loadTexture("ball");

	Vector2 spriteScale = Vector2(10.0f, 10.0f);

	SimpleTransform tempTransform = SimpleTransform(position.x, position.y, spriteScale.x, spriteScale.y, 0);
	SpriteRenderable wallSprite = SpriteRenderable(ballTextureId);

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, wallSprite);
	physicsComponent = transformManager.getPhysicsSystem().createComponentCircle(transform, 9.0f);
}

void EntityBall::launch(Vector2 direction) {
	direction.Normalise();
	float magnitiude = 50.0f;
	physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(magnitiude * direction.x, magnitiude * direction.y), false);
}

void EntityBall::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyRigidBody(physicsComponent);
	transformManager.destroyTransform(transform);
}

void EntityBall::update(EntityContainer& entityManager) {
}
