#include "EntityContactListener.h"
#include "EntityAttempt/PhysicsEntity.h"


EntityContactListener::EntityContactListener()
{
}


EntityContactListener::~EntityContactListener()
{
}

void EntityContactListener::BeginContact(b2Contact* contact) {
	//grab physics entity from body user data
	PhysicsEntity* entityA = static_cast<PhysicsEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	PhysicsEntity* entityB = static_cast<PhysicsEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	ContactData contactData(entityA, entityB, contact);
	contactData.entityCollidingWith = entityB;
	//if user data isn't set, entity will be nullptr here
	if (entityA != nullptr && entityA->acceptsContacts()) {
		//entityA is not a nullptr, entityB might be though so callback will have to check for that
		entityA->onContactBegin(contactData);
	}

	contactData.entityCollidingWith = entityA;
	if (entityB != nullptr && entityB->acceptsContacts()) {
		entityB->onContactBegin(contactData);
	}
}

void EntityContactListener::EndContact(b2Contact* contact) {
	PhysicsEntity* entityA = static_cast<PhysicsEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	PhysicsEntity* entityB = static_cast<PhysicsEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	ContactData contactData(entityA, entityB, contact);
	contactData.entityCollidingWith = entityB;
	if (entityA != nullptr && entityA->acceptsContacts())
	{
		entityA->onContactEnd(contactData);
	}
	contactData.entityCollidingWith = entityA;
	if (entityB != nullptr && entityB->acceptsContacts())
	{
		entityB->onContactEnd(contactData);
	}
}

void EntityContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	PhysicsEntity* entityA = static_cast<PhysicsEntity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	PhysicsEntity* entityB = static_cast<PhysicsEntity*>(contact->GetFixtureB()->GetBody()->GetUserData());
	ContactData contactData(entityA, entityB, contact);
	contactData.entityCollidingWith = entityB;
	if (entityA != nullptr && entityA->acceptsContacts())
	{
		entityA->onContactPreSolve(contactData);
	}
	contactData.entityCollidingWith = entityA;
	if (entityB != nullptr && entityB->acceptsContacts())
	{
		entityB->onContactPreSolve(contactData);
	}
}

void EntityContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}
