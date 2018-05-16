/*
 *Author: Edward Curran
 *Class: PhysicsSystem
 *Description: manages physics components, handles updating box 2d
 *
 *
 *uses a fixed step tick with accumulator and interpolation to deal with variable frame rate
 *code from here:
*https://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
*
*basically a way to deal with render refresh rate not matching physics tick right but
*still using a fixed tick rate
*/


#pragma once

#include "../Box2D/Box2D.h"
#include "MemoryManagement/MemoryPool.h"
#include <vector>
#include "TransformComponents.h"
#include "EntityContactListener.h"

const float FIXED_TIMESTEP = 1.f / 60.0f;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
// Maximum number of steps, to avoid degrading to an halt.
const int MAX_STEPS = 5;
const float SCALE = 20.0f;

const int MAX_NUM_PHYSICS_COMPONENTS = 2000;

struct PhysicsComponent{
	b2Vec2 smoothedPosition;
	b2Vec2 previousPosition;

	//in radians
	float smoothedAngle;
	float previousAngle;

	//store pointer to body this component is attached to
	b2Body* body;

	//store pointer to parent transform
	SimpleTransform* transform;

};

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void update(float dt);

	static b2Vec2 scaleVec2(Vector2 vec2){
		return b2Vec2(vec2.x / SCALE, vec2.y / SCALE);
	}

	static Vector2 scaleB2Vec2(b2Vec2 vec) {
		return Vector2(vec.x * SCALE, vec.y * SCALE);
	}
	

	PhysicsComponent* createComponent(SimpleTransform* transform, b2BodyDef& bodyDef);;
	PhysicsComponent* createComponentBox(SimpleTransform* transform, Vector2 box);
	PhysicsComponent* createComponentCircle(SimpleTransform* transform, float radius);;
	void deleteComponent(PhysicsComponent* body);

protected:

	void singleStep(float);

	void updateControllers(float dt) {};

	//use interpolation for smoothing
	void smoothStates();
	void resetSmoothStates();

	void updateTransforms();

	b2World* world;
	EntityContactListener contactListener;

private:
	
	int numComponents;


	float fixedTimestepAccumulatorRatio;
	float fixedTimestepAccumulator;


	MemoryPool<PhysicsComponent, MAX_NUM_PHYSICS_COMPONENTS * sizeof(PhysicsComponent)> physicsComponentsPool;


	//we actually only need to update dynamicPhysicsComponents
	PhysicsComponent* dynamicComponentList;
	int dynamicComponentCount;
	
};

