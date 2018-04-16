#include "PhysicsSystem.h"
#include "../Box2D/Dynamics/b2Body.h"


PhysicsSystem::PhysicsSystem() : fixedTimestepAccumulatorRatio(0), fixedTimestepAccumulator(0) {
	b2Vec2 gravity(0.0f, -9.81);
	world = new b2World(gravity);

	//we want to handle this manually to clear once per frame
	//not once per step as it would automatically
	world->SetAutoClearForces(false);
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(float dt){
	// Maximum number of steps, to avoid degrading to an halt.
	const int MAX_STEPS = 5;

	fixedTimestepAccumulator += dt;
	const int nSteps = static_cast<int> (
		std::floor(fixedTimestepAccumulator / FIXED_TIMESTEP)
		);
	// To avoid rounding errors, touches fixedTimestepAccumulator_ only
	// if needed.
	if (nSteps > 0)
	{
		fixedTimestepAccumulator -= nSteps * FIXED_TIMESTEP;
	}

	if(fixedTimestepAccumulator < FIXED_TIMESTEP){
		printf("Accumulator must have a value lesser than the fixed time step");
		return;
	}

	fixedTimestepAccumulatorRatio = fixedTimestepAccumulator / FIXED_TIMESTEP;

	// This is similar to clamp "dt":
	//	dt = std::min (dt, MAX_STEPS * FIXED_TIMESTEP)
	// but it allows above calculations of fixedTimestepAccumulator_ and
	// fixedTimestepAccumulatorRatio_ to remain unchanged.
	const int nStepsClamped = std::min(nSteps, MAX_STEPS);
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
}

void PhysicsSystem::singleStep(float dt){
	updateControllers(dt);
	world->Step(dt, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

	//process collisions
}

void PhysicsSystem::smoothStates(){
	const float oneMinusRatio = 1.f - fixedTimestepAccumulatorRatio;

	for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext())
	{
		if (b->GetType() == b2_staticBody)
		{
			continue;
		}

		PhysicsComponent & c = PhysicsComponent::b2BodyToPhysicsComponent(b);
		c.smoothedPosition =
			fixedTimestepAccumulatorRatio * b->GetPosition() +
			oneMinusRatio * c.previousPosition;
		c.smoothedAngle =
			fixedTimestepAccumulatorRatio * b->GetAngle() +
			oneMinusRatio * c.previousAngle;
	}
}

void PhysicsSystem::resetSmoothStates(){
	for (b2Body * b = world->GetBodyList(); b != NULL; b = b->GetNext())
	{
		if (b->GetType() == b2_staticBody)
		{
			continue;
		}

		PhysicsComponent & c = PhysicsComponent::b2BodyToPhysicsComponent(b);
		c.smoothedPosition = b->GetPosition();
		c.smoothedAngle = b->GetAngle();
	}

}
