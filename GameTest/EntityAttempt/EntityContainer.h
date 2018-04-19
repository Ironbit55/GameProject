#pragma once
#include "EntityInterface.h"

const int DELETE_LIST_MAX = 40;
const int MAX_NUM_ENTITIES = 1000;
class EntityContainer
{
public:
	EntityContainer();
	~EntityContainer();

	bool addEntity(EntityInterface* entity) {

	}

	bool deleteEntity() {

	}

	MemoryPool<EntityInterface*, MAX_NUM_ENTITIES * sizeof(EntityInterface*)> entityPool;
};

