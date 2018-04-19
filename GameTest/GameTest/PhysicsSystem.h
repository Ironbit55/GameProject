/*
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

	b2Vec2 scaleVec2(Vector2 vec2){
		return b2Vec2(vec2.x / SCALE, vec2.y / SCALE);
	}
	

	PhysicsComponent* createComponent(SimpleTransform* transform, b2BodyDef& bodyDef, b2FixtureDef& fixtureDef);;
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

	//PhysicsComponent* b2BodyToPhysicsComponent(b2Body* b) {
	//	
	//	return p;
	//};


private:
	
	int numComponents;


	float fixedTimestepAccumulatorRatio;
	float fixedTimestepAccumulator;

	
	
	//i want to use custom allocator to store physics components
	//but i don't have a vector implementation for it
	MemoryPool<PhysicsComponent, MAX_NUM_PHYSICS_COMPONENTS * sizeof(PhysicsComponent)> physicsComponentsPool;



	//so have to use this thing for now
	//std::vector<PhysicsComponent*> physicsComponents;

	//head of linked list
	PhysicsComponent* componentList;
	int componentCount;

	//we actually only need to update dynamicPhysicsComponents
	PhysicsComponent* dynamicComponentList;
	int dynamicComponentCount;

	//so have to use this thing for now
	std::vector<PhysicsComponent*> componentsToDelete;

	
};

