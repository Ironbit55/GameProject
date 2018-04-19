#pragma once
#include "../GameTest/ContentManager.h"
#include "../GameTest/TransformManager.h"




class EntityInterface
{
public:
	EntityInterface(){
		
	};
	~EntityInterface() {};


	void loadContent(ContentManager contentManager);

	virtual void initialise(TransformManager transformManager) = 0;

	virtual void destroy(TransformManager transformManager) {}

	void update();

	void updateDraw();


protected:


};

