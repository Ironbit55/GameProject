#include "PhysicsEntity.h"



PhysicsEntity::PhysicsEntity()
{
}


PhysicsEntity::~PhysicsEntity()
{
}

void PhysicsEntity::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyPhysicsComponent(physicsComponent);
	transformManager.destroyTransform(transform);
}


void PhysicsEntity::createPhysicsComponentFromBody(b2BodyDef& bodyDef, TransformManager& transformManager) {
	physicsComponent = transformManager.attachPhysicsComponent(transform, bodyDef);
	physicsComponent->body->SetUserData(this);
}

void PhysicsEntity::createPhysicsComponentCircle(float radius, TransformManager& transformManager) {
	physicsComponent = transformManager.getPhysicsSystem().createComponentCircle(transform, radius);
	physicsComponent->body->SetUserData(this);
}

void PhysicsEntity::createPhysicsComponentBox(Vector2 box, TransformManager& transformManager) {
	physicsComponent = transformManager.getPhysicsSystem().createComponentBox(transform, box);
	physicsComponent->body->SetUserData(this);
}

void PhysicsEntity::attachFixture(b2FixtureDef& fixtureDef) {
	physicsComponent->body->CreateFixture(&fixtureDef);
}
