#include "PhysicsSystem.h"
#include "../Box2D/Dynamics/b2Body.h"


PhysicsSystem::PhysicsSystem() : fixedTimestepAccumulatorRatio(0), fixedTimestepAccumulator(0) {
	b2Vec2 gravity(0.0f, -9.81);
	world = new b2World(gravity);

	world->SetContactListener(&contactListener);

	//we want to handle this manually to clear once per frame
	//not once per step as it would automatically
	world->SetAutoClearForces(false);
	world->SetAllowSleeping(true);
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(float dt){
	// Maximum number of steps, to avoid degrading to an halt.
	const int MAX_STEPS = 5;

	//convert to seconds
	dt = dt / 1000;
	fixedTimestepAccumulator += dt;
	const int nSteps = static_cast<int> (
			floor(fixedTimestepAccumulator / FIXED_TIMESTEP)
		);
	// To avoid rounding errors, touches fixedTimestepAccumulator_ only
	// if needed.
	if (nSteps > 0)
	{
		fixedTimestepAccumulator -= nSteps * FIXED_TIMESTEP;
	}

	if(fixedTimestepAccumulator > (FIXED_TIMESTEP + FLT_EPSILON)){
		printf("Accumulator must have a value lesser than the fixed time step");
		return;
	}

	fixedTimestepAccumulatorRatio = fixedTimestepAccumulator / FIXED_TIMESTEP;

	// This is similar to clamp "dt":
	//	dt = std::min (dt, MAX_STEPS * FIXED_TIMESTEP)
	// but it allows above calculations of fixedTimestepAccumulator_ and
	// fixedTimestepAccumulatorRatio_ to remain unchanged.
	
	const int nStepsClamped = min(nSteps, MAX_STEPS);
	for (int i = 0; i < nStepsClamped; ++i)
	{
		// In singleStep_() the CollisionManager could fire custom
		// callbacks that uses the smoothed states. So we must be sure
		// to reset them correctly before firing the callbacks.
		resetSmoothStates();
		singleStep(FIXED_TIMESTEP);
	}

	world->ClearForces();

	// We "smooth" positions and orientations using
	// fixedTimestepAccumulatorRatio_ (alpha).
	smoothStates();
	updateTransforms();
}

void PhysicsSystem::singleStep(float dt){
	updateControllers(dt);
	world->Step(dt, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

	//process collisions
}

void PhysicsSystem::smoothStates(){
	const float oneMinusRatio = 1.f - fixedTimestepAccumulatorRatio;

	PhysicsComponent* freeSlot = physicsComponentsPool.firstFree();
	for (PhysicsComponent* component = physicsComponentsPool.first(); component != nullptr; component = physicsComponentsPool.next(component, freeSlot)) {
		b2Body* b = component->body;


		if (b->GetType() == b2_staticBody) {
			continue;
		}

		//update physics component with new smoothed values using previous values
		component->smoothedPosition =
			fixedTimestepAccumulatorRatio * b->GetPosition() +
			oneMinusRatio * component->previousPosition;
		component->smoothedAngle =
			fixedTimestepAccumulatorRatio * b->GetAngle() +
			oneMinusRatio * component->previousAngle;

	}
}

void PhysicsSystem::resetSmoothStates(){
	PhysicsComponent* freeSlot = physicsComponentsPool.firstFree();
	for (PhysicsComponent* component = physicsComponentsPool.first(); component != nullptr; component = physicsComponentsPool.next(component, freeSlot)) {

		b2Body* b = component->body;

		if (b->GetType() == b2_staticBody) {
			continue;
		}

		component->smoothedPosition = b->GetPosition();
		component->smoothedAngle = b->GetAngle();

		if (b->GetType() == b2_staticBody)
		{
			continue;
		}


		component->smoothedPosition = b->GetPosition();
		component->smoothedAngle = b->GetAngle();

	}

}

void PhysicsSystem::updateTransforms(){
	PhysicsComponent* freeSlot = physicsComponentsPool.firstFree();
	for (PhysicsComponent* component = physicsComponentsPool.first(); component != nullptr; component = physicsComponentsPool.next(component, freeSlot)) {
		component->transform->position = Vector2(component->body->GetPosition().x  * SCALE, component->body->GetPosition().y * SCALE);

		float angle = RadToDeg(component->body->GetAngle());
		while (angle <= 0) {
			angle += 360;
		}
		while (angle > 360) {
			angle -= 360;
		}

		//need to normalise this
		component->transform->rotation = angle;
	}
}


PhysicsComponent* PhysicsSystem::createComponent(SimpleTransform* transform, b2BodyDef& bodyDef){
	if(transform == nullptr){
		//can't create physics component without transform
		return nullptr;
	}
	if(numComponents > MAX_NUM_PHYSICS_COMPONENTS){
		cout << "exceeded physics components pool size";
		return nullptr;
	}
	numComponents++;
	PhysicsComponent* component = physicsComponentsPool.allocate();
	
	bodyDef.position = scaleVec2(transform->getOrigin());
	bodyDef.angle = DegToRad(transform->rotation);
	component->body = world->CreateBody(&bodyDef);
	//component->body->CreateFixture(&fixtureDef);
	component->transform = transform;

	return component;

	//only neccesary if component has non default constructor
	//PhysicsComponent* component = physicsComponentsPool.newElement();
}

PhysicsComponent* PhysicsSystem::createComponentBox(SimpleTransform* transform, Vector2 box){
	if(transform == nullptr){
		return nullptr;
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = scaleVec2(transform->getOrigin());

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(box.x / SCALE, box.y / SCALE);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	PhysicsComponent* p = createComponent(transform, bodyDef);
	p->body->CreateFixture(&fixtureDef);
	return p;
	//return createComponent(transform, bodyDef, fixtureDef);
}

PhysicsComponent* PhysicsSystem::createComponentCircle(SimpleTransform* transform, float radius){
	if (transform == nullptr) {
		return nullptr;
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = scaleVec2(transform->getOrigin());

	b2CircleShape circle;
	circle.m_radius = radius / SCALE;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	PhysicsComponent* p = createComponent(transform, bodyDef);
	p->body->CreateFixture(&fixtureDef);

	return p;

	//return createComponent(transform, bodyDef, fixtureDef);
}

void PhysicsSystem::deleteComponent(PhysicsComponent* component){
	numComponents--;
	world->DestroyBody(component->body);
	physicsComponentsPool.deallocate(component);
}
