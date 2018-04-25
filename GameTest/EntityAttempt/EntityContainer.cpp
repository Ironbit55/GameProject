#include "EntityContainer.h"


EntityContainer::~EntityContainer() {
	delete[] entityDeleteList;
}

void EntityContainer::update() {
	entitysize_t* freeSlot = entityPool.firstFree();
	for (entitysize_t* entity = entityPool.first(); entity != nullptr; entity = entityPool.next(entity, freeSlot)) {
		reinterpret_cast<EntityInterface*>(entity)->update(*this);
	}
}

//void EntityContainer::loadContent(ContentManager& contentManager) {
//	entitysize_t* freeSlot = entityPool.firstFree();
//	for (entitysize_t* entity = entityPool.first(); entity != nullptr; entity = entityPool.next(entity, freeSlot)) {
//		reinterpret_cast<EntityInterface*>(entity)->loadContent(contentManager);
//	}
//}
//
//void EntityContainer::initialise(TransformManager& transformManager) {
//	entitysize_t* freeSlot = entityPool.firstFree();
//	
//	for (entitysize_t* entity = entityPool.first(); entity != nullptr; entity = entityPool.next(entity, freeSlot)) {
//		reinterpret_cast<EntityInterface*>(entity)->initialise(transformManager);
//	}
//}

//could do TEMPLATE MAGIC to construct entities of correct type here
EntityInterface* EntityContainer::addEntity(int size) {
	if(size > ENTITY_SIZE_BYTES){
		printf("can't add entity, entity too large: %i", size);
		return nullptr;
	}

	if(numEntities >= MAX_NUM_ENTITIES){
		printf("can't add entity, entity pool is full");
		return nullptr;
	}

	void* destPtr = entityPool.allocate();

	//memcpy(destPtr, entityPtr, size);
	numEntities++;
	EntityInterface* entityDest = reinterpret_cast<EntityInterface*>(destPtr);
	return entityDest;
}

void EntityContainer::deleteEntities(TransformManager& transformManager) {
	//should really sort entities by address before deleting them
	//this is because of the wierd way deallocation works in my memory pool
	for (int i = 0; i < entityDeleteListCount; ++i) {

		(entityDeleteList[i])->destroy(transformManager);
		entityDeleteList[i]->~EntityInterface();

		entitysize_t* entityPtr = (entitysize_t*)entityDeleteList[i];
		entityPool.deallocate(entityPtr);
		numEntities--;
	}

	entityDeleteListCount = 0;
}

void EntityContainer::destructAllEntities(TransformManager& transformManager) {
	entitysize_t* freeSlot = entityPool.firstFree();
	for (entitysize_t* entity = entityPool.first(); entity != nullptr; entity = entityPool.next(entity, freeSlot)) {
		EntityInterface* entityI = reinterpret_cast<EntityInterface*>(entity);
		entityI->destroy(transformManager);
		entityI->~EntityInterface();

		entityPool.deallocate(entity);

		numEntities--;

	}

}
