#pragma once
#include "EntityInterface.h"



class PhysicsEntity;
/*
* set physics entity to nullptr to signal to contact listener
* it has been delete and to not send callbacks for it in the future
*/
struct ContactData {
	ContactData(PhysicsEntity* a, PhysicsEntity* b, b2Contact* contact) : entityA(a), entityB(b), b2Contact(contact) {}
	PhysicsEntity* entityA;
	PhysicsEntity* entityB;
	PhysicsEntity* entityCollidingWith;
	b2Contact* b2Contact;
};

class PhysicsEntity : public EntityInterface
{
public:
	PhysicsEntity();
	~PhysicsEntity();

	void destroy(TransformManager& transformManager) override;
	//virtual void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef& entityDef);

	/*
	 * subclasses override this to create physics component they desire
	 * create a bodydef and use createPhysicsComponentFromBody to
	 * create physics component
	 */
	//virtual void createPhysicsComponent(TransformManager& transformManager) = 0;
	//virtual void createTransform(TransformManager& transformManager) = 0;
	//virtual void createRenderComponent(TransformManager& transformManager) {};


	void createPhysicsComponentFromBody(b2BodyDef& bodyDef, TransformManager& transformManager);
	void createPhysicsComponentCircle(float radius, TransformManager& transformManager);
	void createPhysicsComponentBox(Vector2 box, TransformManager& transformManager);
	void attachFixture(b2FixtureDef& fixtureDef);

	virtual bool acceptsContacts(){
		return false;
	}
	virtual void onContactBegin(const ContactData& data) {};
	virtual void onContactEnd(const ContactData& data) {};
	virtual void onContactPreSolve(const ContactData& data) {};



protected:
	SimpleTransform* transform;
	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;
};

