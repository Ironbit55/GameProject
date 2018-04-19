#pragma once
#include "../GameTest/ContentManager.h"
#include "../Input/InputMapper.h"
#include "../GameTest/MessageReceiver.h"
#include "../Input/InputManager.h"
#include <iostream>
#include "../GameTest/InputReceiver.h"

class Entity : InputReceiver
{
public:
	Entity(){
		registerInput(INPUT_ACTOR_PLAYER1);
	};
	~Entity() {};

	void loadContent(ContentManager contentManager);

	void update();

	//gets called when received a message containing the input for a actor
	//the instance has registered to receive input from
	//inputActor specified which actor the input belongs to
	void handleInput(InputActors inputActor, MappedInput* mappedInput){
		
		if(mappedInput->getAction(InputCooked::ACTION_JUMP)){
			std::cout << "handle input jump!";
		}
	}


protected:
	vector<Entity> children;

private:
	MessageCallback inputMessageCallback;

};

