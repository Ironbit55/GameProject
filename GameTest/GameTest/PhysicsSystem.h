/*
 *code from here:
*https://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
*
*basically a way to deal with render refresh rate not matching physics tick right but
*still using a fixed tick rate
*/


#pragma once
#include "../Box2D/Box2D.h"
#include "../Box2D/Dynamics/b2World.h"

const float FIXED_TIMESTEP = 1.f / 60.f;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
// Maximum number of steps, to avoid degrading to an halt.
const int MAX_STEPS = 5;
const float SCALE = 30.0f;

struct PhysicsComponent{
	b2Vec2 smoothedPosition;
	b2Vec2 previousPosition;

	//in radians
	float smoothedAngle;
	float previousAngle;

	static PhysicsComponent& b2BodyToPhysicsComponent(b2Body* b){
		PhysicsComponent p;
		return p;
	};
};

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(float dt);
	void singleStep(float);

	void updateControllers(float dt) {};

	//use interpolation for smoothing
	void smoothStates();
	void resetSmoothStates();

protected:
	b2World* world;

private:
	float fixedTimestepAccumulatorRatio;
	float fixedTimestepAccumulator;
};

