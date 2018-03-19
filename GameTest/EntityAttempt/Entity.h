#pragma once
#pragma once
#include "../GameTest/ContentManager.h"
#include "../Input/InputMapper.h"
class Entity
{
public:
	Entity() {};
	~Entity() {};

	void loadContent(ContentManager contentManager);

	void update();


	//registers with Input manager as a callback
	virtual void handleInput(MappedInput& mappedInput);

	//registers with Input manager as a callback
	//calls handleInput virtually
	void inputListener(MappedInput& mappedInput) {
		//handleInput(mappedInput);
	}


protected:
	vector<Entity> children;

private:

};

